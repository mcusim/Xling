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
 * Low-level driver for SH1106-based displays connected via bit banging
 * TWI (I2C) interface.
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

/*
 * Types of the TWI control byte.
 *
 * It is a first byte within the command word transmitted to the display
 * over TWI, second one is a data byte.
 *
 *     Bits: Co DC 0 0 0 0 0 0, where:
 *
 *     Co - the continuation bit,
 *     DC - the display command (or graphics RAM) operation bit, i.e.
 *
 *     Co = 0 - the last control byte, only data bytes to follow,
 *     Co = 1 - the next two bytes are a data byte and another control byte,
 *     DC = 0 - the data byte is for command operation,
 *     DC = 1 - the data byte is for RAM operation.
 */
typedef enum TWI_CB {
	CB_LASTCMD = 0x00,	/* 0 0 x x  x x x x */
	CB_LASTRAM = 0x40,	/* 0 1 x x  x x x x */
	CB_CMD = 0x80,		/* 1 0 x x  x x x x */
	CB_RAM = 0xC0,		/* 1 1 x x  x x x x */
} TWI_CB;

/* Most significant nibble */
#define MSN(v)		((uint8_t) (((uint8_t)(v)) & ((uint8_t) 0xF0U)))
/* Last significant nibble */
#define LSN(v)		((uint8_t) (((uint8_t)(v)) & ((uint8_t) 0x0FU)))

/*
 * Maximum number of devices supported by the driver.
 *
 * User is responsible to free display resources when they aren't needed
 * anymore. All driver's memory is statically allocated.
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_DNUM)
#define DNUM		(configMSIM_DRV_DISPLAY_SH1106_DNUM)
#else
#define DNUM		1
#endif

/*
 * Size of the driver's buffer, in bytes.
 *
 * User is responsible to control space left in the buffer.
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_BUFSZ)
#define BUFSZ		(configMSIM_DRV_DISPLAY_SH1106_BUFSZ)
#else
#define BUFSZ		256
#endif

/*
 * An SH1106-based display can be connected by a software implemented
 * (bit banging) two-wire interface (I2C).
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_TWIBB)

/* Forward declaration of the display connection block */
typedef struct sh1106_con sh1106_con;

static void	twibb_init(MSIM_SH1106 *, uint8_t addr,
                           volatile uint8_t *port, volatile uint8_t *ddr,
                           uint8_t sda, uint8_t scl);
static int	twibb_start(sh1106_con *twi);
static int	twibb_stop(sh1106_con *twi);
static void	twibb_write_byte(sh1106_con *twi, uint8_t b);
static void	twibb_write_data(sh1106_con *twi, uint8_t *data, uint32_t len);

static int	append_byte(MSIM_SH1106 *, TWI_CB, const uint8_t);
static int	append_bytes(MSIM_SH1106 *, TWI_CB, const uint8_t *, size_t);
static int	append_bytes_PF(MSIM_SH1106 *, TWI_CB, const uint8_t *, size_t);

/* A display connection block */
struct sh1106_con {
	uint8_t addr;		/* TWI display address */
	volatile uint8_t *port;	/* TWI I/O port (PORTC, PORTD, etc.) */
	volatile uint8_t *ddr;	/* TWI I/O data direction (DDRC, DDRD, etc.) */
	uint8_t sda;		/* SDA pin number of the TWI port */
	uint8_t scl;		/* SCL pin number of the TWI port */
};

/* A display control block (DCB) */
struct MSIM_SH1106 {
	uint8_t init;		/* Display initialized flag */
	sh1106_con con;
	uint8_t buf[BUFSZ];
	uint32_t blen;
};

static struct MSIM_SH1106 devices[DNUM]; /* Pool of the DCBs */

/*
 * Initializes a display (bit-banging TWI).
 *
 * This function returns an opaque pointer to the display control block.
 */
MSIM_SH1106 *
MSIM_SH1106_Init(MSIM_SH1106DisplayConf *conf)
{
	struct MSIM_SH1106 *dev = NULL;

	do {
		if (conf == NULL) {
			break;
		}

		/* Try to find a non-initialized display block. */
		for (uint32_t i = 0; i < DNUM; i++) {
			if (devices[i].init == 0U) {
				dev = &devices[i];
				break;
			}
		}
		if (dev == NULL) {
			break;
		}

		dev->init = 1;
		dev->blen = 0;

		twibb_init(dev, conf->twi_addr, conf->twi_port,
		           conf->twi_ddr, conf->sda, conf->scl);
	} while (0);

	return dev;
}

/*
 * Returns a DCB back to the pool.
 *
 * This function doesn't free any memory. It's up to the driver user to return
 * DCB back to the driver.
 */
void
MSIM_SH1106_Free(MSIM_SH1106 *dev)
{
	if (dev != NULL) {
		dev->init = 0;
		dev->blen = 0;
	}
}

/* Resets length of the driver's buffer. */
int
MSIM_SH1106_ClearBuf(struct MSIM_SH1106 *dev)
{
	int rc = 0;

	if (dev != NULL) {
		dev->blen = 0;
	} else {
		rc = 1;
	}

	return rc;
}

/* Transmits the driver's buffer to the display. */
int
MSIM_SH1106_SendBuf(MSIM_SH1106 *dev)
{
	int rc = 0;

	if (dev != NULL) {
		const uint32_t len = (BUFSZ < dev->blen) ? BUFSZ : dev->blen;
		struct sh1106_con *con = &dev->con;

		twibb_start(con);
		twibb_write_data(con, dev->buf, len);
		twibb_stop(con);
	} else {
		rc = 1;
	}

	return rc;
}

int
MSIM_SH1106_AppendCmd(MSIM_SH1106 *dev, const uint8_t cmd_byte)
{
	return append_byte(dev, CB_CMD, cmd_byte);
}

int
MSIM_SH1106_AppendDat(MSIM_SH1106 *dev, const uint8_t data_byte)
{
	return append_byte(dev, CB_RAM, data_byte);
}

int
MSIM_SH1106_AppendLastCmd(MSIM_SH1106 *dev, const uint8_t *data, size_t len)
{
	return append_bytes(dev, CB_LASTCMD, data, len);
}

int
MSIM_SH1106_AppendLastDat(MSIM_SH1106 *dev, const uint8_t *data, size_t len)
{
	return append_bytes(dev, CB_LASTRAM, data, len);
}

int
MSIM_SH1106_AppendLastDat_PF(MSIM_SH1106 *dev, const uint8_t *d, size_t len)
{
	return append_bytes_PF(dev, CB_LASTRAM, d, len);
}

/*
 * Appends a command to the driver's buffer to change the current
 * page address.
 */
int
MSIM_SH1106_SetPage(struct MSIM_SH1106 *dev, uint8_t page)
{
	return append_byte(dev, CB_CMD, (uint8_t)
	                   (MSN(CMD_PAGEADDR) | ((uint8_t)(page & 0x07U))));
}

/*
 * Appends a command to the driver's buffer to change the current
 * column address.
 */
int
MSIM_SH1106_SetColumn(struct MSIM_SH1106 *dev, uint8_t col)
{
	append_byte(dev, CB_CMD, MSN(CMD_COLHADDR) | LSN(col >> 4));
	return append_byte(dev, CB_CMD, MSN(CMD_COLLADDR) | LSN(col));
}

/* Appends a command to the driver's buffer to switch the display on. */
int
MSIM_SH1106_DisplayOn(struct MSIM_SH1106 *dev)
{
	return append_byte(dev, CB_CMD, CMD_DISPLAYON);
}

/* Appends a command to the driver's buffer to switch the display off. */
int
MSIM_SH1106_DisplayOff(struct MSIM_SH1106 *dev)
{
	return append_byte(dev, CB_CMD, CMD_DISPLAYOFF);
}

int
MSIM_SH1106_SetContrast(struct MSIM_SH1106 *dev, uint8_t val)
{
	append_byte(dev, CB_CMD, CMD_SETCONTRAST);
	return append_byte(dev, CB_CMD, val);
}

int
MSIM_SH1106_DisplayNormal(struct MSIM_SH1106 *dev)
{
	return append_byte(dev, CB_CMD, CMD_NORMALDISPLAY);
}

int
MSIM_SH1106_DisplayInvert(struct MSIM_SH1106 *dev)
{
	return append_byte(dev, CB_CMD, CMD_INVERTDISPLAY);
}

int
MSIM_SH1106_SetStartLine(struct MSIM_SH1106 *dev, uint8_t line)
{
	return append_byte(dev, CB_CMD, (uint8_t)
	                   (CMD_SETSTARTLINE | ((uint8_t)(line & 0x3FU))));
}

int
MSIM_SH1106_SetScanDirection(struct MSIM_SH1106 *dev, uint8_t reverse)
{
	return append_byte(dev, CB_CMD,
	                   (uint8_t) (MSN(CMD_SETSCANDIR) |
	                   (uint8_t) ((reverse << 3) & 0x08)));
}

/* Initializes a bit-banging TWI interface. */
static void
twibb_init(MSIM_SH1106 *dev, uint8_t addr, volatile uint8_t *port,
           volatile uint8_t *ddr, uint8_t sda, uint8_t scl)
{
	sh1106_con * const con = &dev->con;

	/* TWI (bit-banging) configuration */
	con->addr = addr;
	con->port = port;
	con->ddr = ddr;
	con->sda = sda;
	con->scl = scl;

	/* Let them float high */
	(*con->ddr) &= (uint8_t)(~(1 << con->sda));
	(*con->ddr) &= (uint8_t)(~(1 << con->scl));

	/* Pull up both lines */
	(*con->port) = (uint8_t) ((*con->port) | (1 << con->sda));
	(*con->port) = (uint8_t) ((*con->port) | (1 << con->scl));
}

/* Sends a START signal and a slave address over TWI. */
static int
twibb_start(sh1106_con *twi)
{
	(*twi->port) |= (uint8_t)((1 << twi->sda) + (1 << twi->scl));
	(*twi->ddr) |= (uint8_t)((1 << twi->sda) + (1 << twi->scl));

	/* Send START signal */
	(*twi->port) &= (uint8_t)(~(1 << twi->sda));
	(*twi->port) &= (uint8_t)(~(1 << twi->scl));

	/* Send the slave address */
	twibb_write_byte(twi, (uint8_t) (twi->addr << 1));

	return 0;
}

/* Sends a STOP signal over TWI. */
static int
twibb_stop(sh1106_con *twi)
{
	/* Send STOP signal */
	(*twi->port) &= (uint8_t)(~(1 << twi->sda));
	(*twi->port) = (uint8_t)((*twi->port) | (1 << twi->scl));
	(*twi->port) = (uint8_t)((*twi->port) | (1 << twi->sda));

	/* Let the lines float (tri-state) */
	(*twi->ddr) &= (uint8_t)((1 << twi->sda) | (1 << twi->scl));

	return 0;
}

static void
twibb_write_byte(sh1106_con *twi, uint8_t b)
{
	uint8_t i;
	uint8_t bOld = (uint8_t)
	        ((*twi->port) & (~((1 << twi->sda)|(1 << twi->scl))));

	for (i = 0; i < 8; i++) {
		bOld &= (uint8_t)(~(1 << twi->sda));
		if (b & 0x80) {
			bOld = (uint8_t)(bOld | (1 << twi->sda));
		}
		(*twi->port) = bOld;
		(*twi->port) = (uint8_t)((*twi->port) | (1 << twi->scl));
		(*twi->port) = bOld;
		b = (uint8_t)(b << 1);
	}

	/* ACK bit */
	/* Set data low */
	(*twi->port) = (uint8_t)(bOld & (~(1 << twi->sda)));

	/* Toggle clock */
	(*twi->port) = (uint8_t)((*twi->port) | (1 << twi->scl));
	(*twi->port) = bOld;
}

static void
twibb_write_data(sh1106_con *twi, uint8_t *data, uint32_t len)
{
	uint8_t i, b;
	uint8_t bOld = (uint8_t)
	        ((*twi->port) & (~((1 << twi->sda)) | (1 << twi->scl)));

	while (len--) {
		b = *data++;
		if (b == 0 || b == 0xff) {
			/* Special case can save time */
			bOld &= (uint8_t)(~(1 << twi->sda));
			if (b & 0x80) {
				bOld = (uint8_t)(bOld | (1 << twi->sda));
			}
			(*twi->port) = bOld;

			for (i = 0; i < 8; i++) {
				/* Just toggle SCL, SDA stays the same */
				(*twi->port) = (uint8_t)
				        ((*twi->port) | (1 << twi->scl));
				(*twi->port) = bOld;
			}
		} else {
			/* Normal byte needs every bit tested */
			for (i = 0; i < 8; i++) {
				bOld &= (uint8_t)(~(1 << twi->sda));
				if (b & 0x80) {
					bOld = (uint8_t)
					        (bOld | (1 << twi->sda));
				}
				(*twi->port) = bOld;
				(*twi->port) = (uint8_t)
				        ((*twi->port) | (1 << twi->scl));
				(*twi->port) = bOld;
				b = (uint8_t)(b << 1);
			}
		}

		/*
		 * ACK bit seems need to be set to 0, but SDA line
		 * doesn't need to be tri-state.
		 */
		(*twi->port) &= (uint8_t)(~(1 << twi->sda));

		/* Toggle clock */
		(*twi->port) = (uint8_t)((*twi->port) | (1 << twi->scl));
		(*twi->port) &= (uint8_t)(~(1 << twi->scl));
	}
}

static int
append_byte(MSIM_SH1106 *dev, MSIM_SH1106_TWICB cb, const uint8_t byte)
{
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ - 1))) {
		dev->buf[dev->blen] = cb;
		dev->buf[dev->blen+1] = byte;
		dev->blen += 2;
	} else {
		rc = MSIM_SH1106_RC_NULLPTR;
	}

	return rc;
}

static int
append_bytes(MSIM_SH1106 *dev, MSIM_SH1106_TWICB cb, const uint8_t *data,
             size_t len)
{
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (len <= (BUFSZ - dev->blen + 1))) {
		dev->buf[dev->blen] = cb;
		memcpy((void *) &dev->buf[dev->blen + 1], data, len);
		dev->blen += len + 1;
	} else {
		rc = MSIM_SH1106_RC_NULLPTR;
	}

	return rc;
}

static int
append_bytes_PF(MSIM_SH1106 *dev, MSIM_SH1106_TWICB cb, const uint8_t *data,
                size_t len)
{
	int rc = MSIM_SH1106_RC_OK;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (len <= (BUFSZ - dev->blen + 1))) {
		dev->buf[dev->blen] = cb;
		memcpy_PF((void *) &dev->buf[dev->blen + 1], data, len);
		dev->blen += len + 1;
	} else {
		rc = MSIM_SH1106_RC_NULLPTR;
	}

	return rc;
}

#endif /* defined(configMSIM_DRV_DISPLAY_SH1106_TWIBB) */
