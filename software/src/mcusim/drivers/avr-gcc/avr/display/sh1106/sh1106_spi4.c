/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of MCUSim, an XSPICE library with microcontrollers.
 *
 * Copyright (C) 2017-2019 MCUSim Developers
 *
 * MCUSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MCUSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*
 * Implementation of the low-level driver for SH1106-based displays connected
 * via 4-wire SPI interface.
 *
 * NOTE: An interrupt of the SPI serial transfer (SPI_STC_vect) is occupied by
 * the driver.
 */

#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106.h"

/* Fundamental display commands */
#define CMD_DISPLAYOFF		((uint8_t) 0xAEU)
#define CMD_DISPLAYON		((uint8_t) 0xAFU)
#define CMD_SETCONTRAST		((uint8_t) 0x81U)
#define CMD_NORMALDISPLAY	((uint8_t) 0xA6U)
#define CMD_INVERTDISPLAY	((uint8_t) 0xA7U)
#define CMD_SETSTARTLINE	((uint8_t) 0x40U) /* prefix for 0x40-0x7F */
#define CMD_PAGEADDR		((uint8_t) 0xB0U) /* prefix for 0xB0-0xB7 */
#define CMD_COLHADDR		((uint8_t) 0x10U)
#define CMD_COLLADDR		((uint8_t) 0x00U)
#define CMD_SETSCANDIR		((uint8_t) 0xC0U)

/*
 * Pump voltage commands.
 * Specifies output voltage (Vpp) of the internal charger pump.
 */
#define CMD_PUMPV_6V4		((uint8_t) 0x30U) /* Vpump = 6.4V */
#define CMD_PUMPV_7V4		((uint8_t) 0x31U) /* Vpump = 7.4V */
#define CMD_PUMPV_8V0		((uint8_t) 0x32U) /* Vpump = 8.0V */
#define CMD_PUMPV_9V0		((uint8_t) 0x33U) /* Vpump = 9.0V */

/* Most significant nibble */
#define MSN(v)		((uint8_t) (((uint8_t)(v)) & ((uint8_t) 0xF0U)))
/* Last significant nibble */
#define LSN(v)		((uint8_t) (((uint8_t)(v)) & ((uint8_t) 0x0FU)))

/* Bits manipulation macros */
#define SET_BIT(b, bt)		((b) = (uint8_t)((b) | (1U << (bt))))
#define CLEAR_BIT(b, bt)	((b) &= (uint8_t) ~(1U << (bt)))

/*
 * Maximum number of the devices supported by the driver.
 * User is responsible to free display resources when they aren't needed
 * anymore. All driver's memory is statically allocated.
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_DNUM)
#define DNUM		(configMSIM_DRV_DISPLAY_SH1106_DNUM)
#else
#define DNUM		2
#endif

/*
 * Size of the driver buffer, in bytes.
 * User is responsible to control space left in the buffer.
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_BUFSZ)
#define BUFSZ		(configMSIM_DRV_DISPLAY_SH1106_BUFSZ)
#else
#define BUFSZ		256
#endif

/* An SH1106-based display can be connected by a 4-wire SPI. */
#if defined(configMSIM_DRV_DISPLAY_SH1106_SPI4)

/* Wait for connection ready state */
#define WAIT_TILL_READY(d)	while (((cdev) != NULL) || (((d) != NULL) && \
                                       ((d)->state != CON_READY) && \
                                       ((d)->state != CON_INIT))){_delay_us(5);}

/* Types of the data located in the driver's buffer */
typedef enum buf_dtype {
	DC_UNKNOWN,		/* Unknown type */
	DC_CMD,			/* Data for command register */
	DC_GRAM,		/* Data for graphics RAM */
} buf_dtype;

/* SPI connection state */
typedef enum con_state {
	CON_INIT,		/* Connection has been initialized */
	CON_READY,		/* Connection is ready */
	CON_MASTER_TRANS,	/* Master transmits data */
	CON_MASTER_RECV,	/* Master receives data */
} con_state;

/* Cconnection block */
typedef struct sh1106_con {
	volatile uint8_t	*rst_port;	/* RST port */
	volatile uint8_t	*rst_ddr;	/* RST port (DDR) */
	volatile uint8_t	*cs_port;	/* CS port */
	volatile uint8_t	*cs_ddr;	/* CS port (DDR) */
	volatile uint8_t	*dc_port;	/* D/C port */
	volatile uint8_t	*dc_ddr;	/* D/C port (DDR) */
	uint8_t			rst;		/* Reset pin # */
	uint8_t			cs;		/* Chip-select pin # */
	uint8_t			dc;		/* Data/Command pin # */
} sh1106_con;

/* Display control block (DCB) */
struct MSIM_SH1106_t {
	volatile sh1106_con	con;		/* Display's connection */
	volatile uint16_t	bytes_len;	/* Buffer length (in bytes) */
	volatile uint16_t	sent_i;		/* Byte index to send */
	volatile uint16_t	sent_len;	/* # of bytes to send */
	volatile con_state	state;		/* Display's connection state */
	volatile buf_dtype	dat_type;	/* Buffer data type */
	volatile uint8_t	init;		/* Display initialized flag */
	volatile uint8_t	buf[BUFSZ];	/* DCB's buffer */
};

/* Local function declarations. */
static int append_byte(MSIM_SH1106_t *, buf_dtype, const uint8_t);
static int append_bytes(MSIM_SH1106_t *, buf_dtype, const uint8_t *, size_t);
static int append_bytes_PF(MSIM_SH1106_t *, buf_dtype, const uint8_t *, size_t);

/* "Private" variables for the driver. */
static uint8_t drv_init;			/* A "driver init" flag */
static volatile MSIM_SH1106_t * volatile cdev;	/* Current DCB (for ISR) */
static MSIM_SH1106_t devices[DNUM];		/* Pool of the DCBs */

/*
 * Initializes the driver.
 */
int
MSIM_SH1106__drvStart(const MSIM_SH1106DrvConf_t *conf)
{
	/* Set MOSI and SCK output */
	SET_BIT((*conf->ddr_spi), conf->mosi);
	SET_BIT((*conf->ddr_spi), conf->sck);

	/* Enable SPI, its interrupt, master mode, and SPI mode 3. */
	SPCR |= (1 << SPE) | (1 << SPIE) | (1 << MSTR) |
	        (1 << CPHA) | (1 << CPOL);

	/* SPI clock to Fosc/4 */
	CLEAR_BIT(SPCR, SPR1);
	CLEAR_BIT(SPCR, SPR0);
	CLEAR_BIT(SPSR, SPI2X);

	/* Driver shouldn't be aware of any DCB yet. */
	cdev = NULL;

	/* Driver has been initialized. */
	drv_init = 1;

	return MSIM_SH1106_RC_OK;
}

/*
 * Performs a de-initialization of the driver.
 */
int
MSIM_SH1106__drvStop(void)
{
	/* Disable SPI, its interrupt */
	CLEAR_BIT(SPCR, SPE);
	CLEAR_BIT(SPCR, SPIE);

	/* Driver has been de-initialized */
	drv_init = 0;

	/* Driver shouldn't be aware of any DCB now. */
	cdev = NULL;

	return MSIM_SH1106_RC_OK;
}

/*
 * Initializes a display and its control block.
 *
 * This function returns an opaque pointer to the DCB.
 */
MSIM_SH1106_t *
MSIM_SH1106_Init(const MSIM_SH1106Conf_t *conf)
{
	MSIM_SH1106_t *dev = NULL;

	do {
		/* Can't initialize the display without a config */
		if (conf == NULL) {
			break;
		}

		/* Trying to find a non-initialized display block. */
		for (uint32_t i = 0; i < DNUM; i++) {
			if (devices[i].init == 0U) {
				dev = &devices[i];
				break;
			}
		}

		if (dev == NULL) {
			break; /* Driver doesn't have a DCB available. */
		}

		/* Default DCB configuration */
		dev->init = 1;
		dev->bytes_len = 0;
		dev->dat_type = DC_UNKNOWN;
		dev->sent_i = 0;
		dev->sent_len = 0;

		/* Copy reset (RST) pin config */
		dev->con.rst_port = conf->rst_port;
		dev->con.rst_ddr = conf->rst_ddr;
		dev->con.rst = conf->rst;
		/* Copy chip-select (CS) pin config */
		dev->con.cs_port = conf->cs_port;
		dev->con.cs_ddr = conf->cs_ddr;
		dev->con.cs = conf->cs;
		/* Copy data/command (D/C) pin config */
		dev->con.dc_port = conf->dc_port;
		dev->con.dc_ddr = conf->dc_ddr;
		dev->con.dc = conf->dc;

		/* Set RST, CS and D/C pins to output */
		SET_BIT((*dev->con.rst_ddr), dev->con.rst);
		SET_BIT((*dev->con.cs_ddr), dev->con.cs);
		SET_BIT((*dev->con.dc_ddr), dev->con.dc);

		/* Connection has been successfully initialized. */
		dev->state = CON_INIT;
	} while (0);

	return dev;
}

/*
 * Returns a DCB back to the pool.
 *
 * This function doesn't free any memory. It's up to the user to return DCB
 * back to the driver.
 */
void
MSIM_SH1106_Free(MSIM_SH1106_t *dev)
{
	WAIT_TILL_READY(dev);

	if (dev != NULL) {
		dev->init = 0;
		dev->bytes_len = 0;
		dev->dat_type = DC_UNKNOWN;
	}
}


/*
 * A busy-wait for the connection ready state.
 */
void
MSIM_SH1106_Wait(MSIM_SH1106_t *dev)
{
	WAIT_TILL_READY(dev);
}

/*
 * Resets length of the DCB's buffer.
 */
int
MSIM_SH1106_bufClear(MSIM_SH1106_t *dev)
{
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	if (dev != NULL) {
		dev->bytes_len = 0;
		dev->dat_type = DC_UNKNOWN;
	} else {
		rc = MSIM_SH1106_RC_NULLPTR;
	}

	return rc;
}

/*
 * Transmits the driver's buffer to the display.
 */
int
MSIM_SH1106_bufSend(MSIM_SH1106_t *dev)
{
	int rc = MSIM_SH1106_RC_OK;

	do {
		if (dev == NULL) {
			/* Do nothing if DCB is not available */
			rc = MSIM_SH1106_RC_NULLPTR;
			break;
		}
		if (cdev != NULL) {
			/* Check the driver's state */
			rc = MSIM_SH1106_RC_DRIVERNOTREADY;
			break;
		}
		if ((dev->state != CON_READY) && (dev->state != CON_INIT)) {
			/* Check the connection state */
			rc = MSIM_SH1106_RC_DEVICENOTREADY;
			break;
		}

		/* Adjust a D/C pin */
		if (dev->dat_type == DC_CMD) {
			CLEAR_BIT((*dev->con.dc_port), dev->con.dc);
		} else if (dev->dat_type == DC_GRAM) {
			SET_BIT((*dev->con.dc_port), dev->con.dc);
		} else {
			/* ERROR: Data type should be known! */
			rc = MSIM_SH1106_RC_UNKNOWNDATTYPE;
			break;
		}

		/* Pull CS low to enable the serial interface of the display */
		CLEAR_BIT((*dev->con.cs_port), dev->con.cs);

		/* The driver should be aware of the current DCB. */
		cdev = dev;

		/* Switch to Master Transmitter mode... */
		dev->state = CON_MASTER_TRANS;
		dev->sent_len = dev->bytes_len;
		dev->sent_i = 1;
		/* ... and send the first payload byte. */
		SPDR = dev->buf[0];
	} while (0);

	return rc;
}

/*
 * Appends a data byte (for graphics RAM) to the DCB's buffer.
 */
int
MSIM_SH1106_bufAppend(MSIM_SH1106_t *dev, const uint8_t data_byte)
{
	return append_byte(dev, DC_GRAM, data_byte);
}

/*
 * Appends data bytes (for graphics RAM) to the DCB's buffer.
 */
int
MSIM_SH1106_bufAppendLast(MSIM_SH1106_t *dev, const uint8_t *data, size_t len)
{
	return append_bytes(dev, DC_GRAM, data, len);
}

/*
 * Appends data bytes (for graphics RAM, located in the program memory) to the
 * DCB's buffer.
 */
int
MSIM_SH1106_bufAppendLast_PF(MSIM_SH1106_t *dev, const uint8_t *data, size_t len)
{
	return append_bytes_PF(dev, DC_GRAM, data, len);
}

/*
 * Specifies page address to load display RAM data to page address register.
 */
int
MSIM_SH1106_SetPage(MSIM_SH1106_t *dev, uint8_t page)
{
	return append_byte(dev, DC_CMD, (uint8_t)
	                   (MSN(CMD_PAGEADDR) | ((uint8_t)(page & 0x07U))));
}

/*
 * Specifies column address (0-127 for 128x64 OLED, for instance) of the
 * display RAM.
 */
int
MSIM_SH1106_SetColumn(MSIM_SH1106_t *dev, uint8_t col)
{
	append_byte(dev, DC_CMD, MSN(CMD_COLHADDR) | LSN(col >> 4));
	return append_byte(dev, DC_CMD, MSN(CMD_COLLADDR) | LSN(col));
}

/*
 * Switch the display on.
 */
int
MSIM_SH1106_DisplayOn(MSIM_SH1106_t *dev)
{
	return append_byte(dev, DC_CMD, CMD_DISPLAYON);
}

/*
 * Switch the display off.
 */
int
MSIM_SH1106_DisplayOff(MSIM_SH1106_t *dev)
{
	return append_byte(dev, DC_CMD, CMD_DISPLAYOFF);
}

/*
 * Set the display's contrast value.
 */
int
MSIM_SH1106_SetContrast(MSIM_SH1106_t *dev, uint8_t val)
{
	append_byte(dev, DC_CMD, CMD_SETCONTRAST);
	return append_byte(dev, DC_CMD, val);
}

/*
 * Switch the display to the "normal" state (1 - pixel's ON, 0 - pixel's OFF).
 */
int
MSIM_SH1106_DisplayNormal(MSIM_SH1106_t *dev)
{
	return append_byte(dev, DC_CMD, CMD_NORMALDISPLAY);
}

/*
 * Switch the display to the "inverted" state (1 - pixel's OFF, 0 - pixel's ON).
 */
int
MSIM_SH1106_DisplayInvert(MSIM_SH1106_t *dev)
{
	return append_byte(dev, DC_CMD, CMD_INVERTDISPLAY);
}

/*
 * Specifies line address (0-63 for 128x64 OLED, for instance) to determine the
 * initial display line or COM0.
 *
 * The RAM display data becomes the top line of the OLED screen.
 */
int
MSIM_SH1106_SetStartLine(MSIM_SH1106_t *dev, uint8_t line)
{
	return append_byte(dev, DC_CMD, (uint8_t)
	                   (CMD_SETSTARTLINE | ((uint8_t)(line & 0x3FU))));
}

/*
 * This command sets the scan direction of the common output allowing layout
 * flexibility in OLED module design. If this command is sent during normal
 * operation, the graphic display will be flipped vertically.
 */
int
MSIM_SH1106_SetScanDirection(MSIM_SH1106_t *dev, uint8_t rvrs)
{
	return append_byte(dev, DC_CMD, (uint8_t)
	                   (MSN(CMD_SETSCANDIR) | (uint8_t)((rvrs << 3)&0x08)));
}

/*
 * Appends a byte of the specified type (command or data) to the DCB's buffer.
 *
 * NOTE: Data of the same type should be placed in the buffer only, and it is
 * selected after a buffer clean call and the first byte/bytes appended.
 */
static int
append_byte(MSIM_SH1106_t *dev, buf_dtype dc, const uint8_t byte)
{
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	do {
		/* Can't operate without a DCB */
		if (dev == NULL) {
			rc = MSIM_SH1106_RC_NULLPTR;
			break;
		}
		/* There should be a place in the buffer */
		if (1 > (BUFSZ - dev->bytes_len)) {
			rc = MSIM_SH1106_RC_NOBUFSPACE;
			break;
		}
		/* Check data type of the buffer */
		if ((dev->dat_type != DC_UNKNOWN) && (dev->dat_type != dc)) {
			rc = MSIM_SH1106_RC_WRONGDATTYPE;
			break;
		}

		/* Append a payload byte. */
		dev->buf[dev->bytes_len] = byte;
		dev->bytes_len++;
		dev->dat_type = dc;
	} while (0);

	return rc;
}

/*
 * Appends bytes of the specified type (command or data) to the DCB's buffer.
 *
 * NOTE: Data of the same type should be placed in the buffer only, and it is
 * selected after a buffer clean call and the first byte/bytes appended.
 */
static int
append_bytes(MSIM_SH1106_t *dev, buf_dtype dc, const uint8_t *data, size_t len)
{
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	do {
		/* Can't operate without a DCB */
		if (dev == NULL) {
			rc = MSIM_SH1106_RC_NULLPTR;
			break;
		}
		/* There should be a place in the buffer for (2 * len) bytes */
		if (len > (BUFSZ - dev->bytes_len)) {
			rc = MSIM_SH1106_RC_NOBUFSPACE;
			break;
		}
		/* Check data type of the buffer */
		if ((dev->dat_type != DC_UNKNOWN) && (dev->dat_type != dc)) {
			rc = MSIM_SH1106_RC_WRONGDATTYPE;
			break;
		}

		memcpy((void *) &dev->buf[dev->bytes_len], data, len);
		dev->bytes_len += len;
		dev->dat_type = dc;
	} while (0);

	return rc;
}

/*
 * Appends bytes (located in the program memory) of the specified type
 * (command or data) to the DCB's buffer.
 *
 * NOTE: Data of the same type should be placed in the buffer only, and it is
 * selected after a buffer clean call and the first byte/bytes appended.
 */
static int
append_bytes_PF(MSIM_SH1106_t *dev, buf_dtype dc, const uint8_t *d, size_t len)
{
	const uint_farptr_t pdat = (uint_farptr_t) d;
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	do {
		/* Can't operate without a DCB */
		if (dev == NULL) {
			rc = MSIM_SH1106_RC_NULLPTR;
			break;
		}
		/* There should be a place in the buffer for (2 * len) bytes */
		if (len > (BUFSZ - dev->bytes_len)) {
			rc = MSIM_SH1106_RC_NOBUFSPACE;
			break;
		}
		/* Check data type of the buffer */
		if ((dev->dat_type != DC_UNKNOWN) && (dev->dat_type != dc)) {
			rc = MSIM_SH1106_RC_WRONGDATTYPE;
			break;
		}

		memcpy_PF((void *) &dev->buf[dev->bytes_len], pdat, len);
		dev->bytes_len += len;
		dev->dat_type = dc;
	} while (0);

	return rc;
}

/*
 * An ISR which is triggered when the SPI serial transfer has been completed.
 */
ISR(SPI_STC_vect)
{
	/*
	 * Driver should be aware of the current DCB which is used to transmit
	 * data to the display, and there should be bytes in the buffer ready
	 * to be sent.
	 */
	if ((cdev != NULL) && (cdev->sent_i < cdev->sent_len)) {
		/* Send the next payload byte */
		SPDR = cdev->buf[cdev->sent_i];
		cdev->sent_i++;
	} else {
		/* Pull CS high to reset the serial interface */
		SET_BIT((*cdev->con.cs_port), cdev->con.cs);

		/* Connection is ready now */
		cdev->state = CON_READY;

		/*
		 * Driver doesn't need to hold a pointer to a specific DCB
		 * anymore.
		 */
		cdev = NULL;
	}
}

#endif /* defined(configMSIM_DRV_DISPLAY_SH1106_SPI4) */
