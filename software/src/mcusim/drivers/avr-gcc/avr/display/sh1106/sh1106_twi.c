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
 * Low-level driver for SH1106-based displays connected via TWI interface.
 *
 * NOTE: An interrupt of the TWI interface (TWI_vect) is occupied by the driver.
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
 * TWI control byte.
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
typedef enum MSIM_SH1106_TWICB {
	CB_LASTCMD = 0x00,	/* 0 0 x x  x x x x */
	CB_LASTRAM = 0x40,	/* 0 1 x x  x x x x */
	CB_CMD = 0x80,		/* 1 0 x x  x x x x */
	CB_RAM = 0xC0,		/* 1 1 x x  x x x x */
} MSIM_SH1106_TWICB;

/* Most significant nibble */
#define MSN(v)		((uint8_t) (((uint8_t)(v)) & ((uint8_t) 0xF0U)))
/* Last significant nibble */
#define LSN(v)		((uint8_t) (((uint8_t)(v)) & ((uint8_t) 0x0FU)))

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

/*
 * An SH1106-based display can be connected by a hardware implemented two-wire
 * interface (I2C).
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_TWI)

/* Status of the TWI hardware */
#define TWI_STATUS		((uint8_t)(TWSR & 0xF8U))

/*
 * TWI operations.
 *
 *     RESUME_TWI -   Starts the operation of the TWI by clearing
 *                    the TWINT flag.
 *     SEND_START -   Sends the START signal, enables interrupts and TWI,
 *                    clears TWINT flag to resume a transfer.
 *     SEND_STOP -    Sends the STOP signal, enables interrupts and TWI, clears
 *                    TWINT flag.
 *     RESUME_TRANS - Used to resume a transfer, clear TWINT and ensure that
 *                    TWI and interrupts are enabled.
 *     SEND_ACK -     (MR mode) Resumes a transfer, ensures that TWI and
 *                    interrupts are enabled and responds with ACK if
 *                    the device is addressed as a slave or after it received
 *                    a byte.
 *     SEND_NACK -    (MR mode) Resumes a transfer, ensures that TWI and
 *                    interrupts are enabled but DO NOT respond with ACK if
 *                    the device is addressed as a slave or after it received
 *                    a byte.
 */
#define RESUME_TWI()	(TWCR |= (1<<TWINT))
#define SEND_START()	(TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTA))
#define SEND_STOP()	(TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTO))
#define RESUME_TRANS()	(TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE))
#define SEND_ACK()	(TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA))
#define SEND_NACK()	(TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE))

/*
 * TWI Status Codes (for hardware-implemented TWI only), where
 *
 *     MT - Master Transmitter Mode
 *     MR - Master Receiver Mode
 *
 * Miscellaneous State codes:
 *
 *     0x08 - A START condition has been transmitted.
 *     0x10 - A repeated START condition has been transmitted.
 *     0x38 - Arbitration lost in SLA+W, SLA+R or data bytes.
 *     0xF8 - No relevant state information available.
 *     0x00 - Bus error due to an illegal START or STOP condition.
 *
 * Status codes for Master Transmitter Mode:
 *
 *     0x18 - SLA+W has been transmitted; ACK has been received
 *            (where SLA - slave address, W - write bit).
 *     0x20 - SLA+W has been transmitted; NOT ACK has been received.
 *     0x28 - Data byte has been transmitted; ACK has been received.
 *     0x30 - Data byte has been transmitted; NOT ACK has been received.
 */
#define START_SENT		((uint8_t) 0x08U)
#define RSTART_SENT		((uint8_t) 0x10U)
#define ARB_LOST		((uint8_t) 0x38U)
#define NO_INFO			((uint8_t) 0xF8U) /* Initial value of TWSR */
#define BUS_ERROR		((uint8_t) 0x00U)

#define MT_SLAW_ACK		((uint8_t) 0x18U)
#define MT_SLAW_NACK		((uint8_t) 0x20U)
#define MT_DATA_ACK		((uint8_t) 0x28U)
#define MT_DATA_NACK		((uint8_t) 0x30U)

/* Wait for connection ready state */
#define WAIT_TILL_READY(d)	while (((d)->state != CON_READY) && \
                                       ((d)->state != CON_INIT)) {_delay_us(5);}

/* TWI connection state */
enum sh1106_con_state {
	CON_INIT,			/* Connection has been initialized */
	CON_READY,			/* Connection is ready */
	CON_MASTER_TRANS,		/* Master transmits data via TWI */
	CON_MASTER_RECV,		/* Master receives data via TWI */
};

/* A display control block (DCB) */
struct MSIM_SH1106 {
	volatile enum sh1106_con_state state;
	volatile uint8_t init;		/* Display initialized flag */
	volatile uint8_t addr;		/* TWI address of the display */
	volatile uint32_t blen;		/* Current buffer length */
	volatile uint32_t sent_i;	/* Current byte index to send */
	volatile uint32_t sent_len;	/* Number of bytes to send */
	uint8_t buf[BUFSZ];		/* Buffer for commands and data */
};

static uint8_t drv_init = 0;			/* Driver initialized flag */
static struct MSIM_SH1106 *cdev;		/* Current DCB (for ISR) */
static struct MSIM_SH1106 devices[DNUM];	/* Pool of the DCBs */

/* Initializes the driver (TWI). */
int
MSIM_SH1106__drvStart(struct MSIM_SH1106DriverConf *conf)
{
	/* TWI Status to NO_INFO, TWI prescaler - to 1 */
	TWSR = NO_INFO;
	/* Set TWI Bit Rate register */
	TWBR = (uint8_t)(((conf->cpu_f / conf->twi_f) - 16) / 2);
	/* Enable TWI and its interrupt */
	TWCR = (1 << TWEN) | (1 << TWIE);

	drv_init = 1;
	cdev = NULL;

	return MSIM_SH1106_RC_OK;
}

/* Performs a de-initialization of the driver (TWI). */
int
MSIM_SH1106__drvStop(void)
{
	/* TWI Status to NO_INFO, TWI prescaler - to 1 */
	TWSR = NO_INFO;
	/* Disable TWI and its interrupt */
	TWCR &= (uint8_t)(~((1 << TWEN) | (1 << TWIE)));

	drv_init = 0;
	cdev = NULL;

	return MSIM_SH1106_RC_OK;
}

/*
 * Initializes a display (TWI).
 * This function returns an opaque pointer which is used to control the display.
 */
struct MSIM_SH1106 *
MSIM_SH1106_Init(struct MSIM_SH1106DisplayConf *conf)
{
	struct MSIM_SH1106 *dev = NULL;

	do {
		/* Do nothing if the driver isn't initialized yet. */
		if (drv_init != 1) {
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
			break; /* Driver doesn't have a DCB available. */
		}

		/* Default DCB configuration */
		dev->init = 1;
		dev->blen = 0;
		dev->sent_i = 0;
		dev->sent_len = 0;
		dev->addr = conf->twi_addr;

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
MSIM_SH1106_Free(MSIM_SH1106 *dev)
{
	if (dev != NULL) {
		dev->init = 0;
		dev->blen = 0;
	}
}

int
MSIM_SH1106_bufSend(MSIM_SH1106 *dev)
{
	int rc = MSIM_SH1106_RC_OK;

	do {
		/* Do nothing if can't access DCB */
		if (dev == NULL) {
			rc = MSIM_SH1106_RC_NULLPTR;
			break;
		}
		/* Check the driver state */
		if (cdev != NULL) {
			rc = MSIM_SH1106_RC_DRIVERNOTREADY;
			break;
		}
		/* Check the connection state */
		if ((dev->state != CON_READY) && (dev->state != CON_INIT)) {
			rc = MSIM_SH1106_RC_DEVICENOTREADY;
			break;
		}

		/* The driver should be aware of the current DCB. */
		cdev = dev;

		/* Switch to Master Transmitter mode... */
		dev->state = CON_MASTER_TRANS;
		dev->sent_i = 0;
		dev->sent_len = dev->blen;
		/* ... and send START via TWI. */
		SEND_START();
	} while (0);

	return rc;
}

/* Resets length of the driver's buffer. */
int
MSIM_SH1106_bufClear(MSIM_SH1106 *dev)
{
	int rc = 0;

	if (dev != NULL) {
		dev->blen = 0;
	} else {
		rc = 1;
	}

	return rc;
}

/* Appends a byte to the driver's buffer. */
int
MSIM_SH1106_Write(struct MSIM_SH1106 *dev, uint8_t byte)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < BUFSZ)) {
		dev->buf[dev->blen] = byte;
		dev->blen++;
	} else {
		rc = 1;
	}

	return rc;
}

/* Appends data of the arbitrary length to the driver's buffer. */
int
MSIM_SH1106_WriteData(struct MSIM_SH1106 *dev, const uint8_t *data, size_t len)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (len <= (BUFSZ - dev->blen))) {
		memcpy((void *) &dev->buf[dev->blen], data, len);
		dev->blen += len;
	} else {
		rc = 1;
	}

	return rc;
}

/*
 * Appends data of the arbitrary length to the driver's buffer.
 * It copies a memory block from flash to SRAM.
 */
int
MSIM_SH1106_WriteData_PF(struct MSIM_SH1106 *dev, const uint8_t *d, size_t len)
{
	const uint_farptr_t pdat = (uint_farptr_t) d;
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (len <= (BUFSZ - dev->blen))) {
		memcpy_PF((void *) &dev->buf[dev->blen], pdat, len);
		dev->blen += len;
	} else {
		rc = 1;
	}

	return rc;
}

/*
 * Appends a command to the driver's buffer to change the current
 * page address.
 */
int
MSIM_SH1106_SetPage(struct MSIM_SH1106 *dev, uint8_t page)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = (uint8_t)
		        (MSN(CMD_PAGEADDR) | ((uint8_t)(page & 0x07U)));
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

/*
 * Appends a command to the driver's buffer to change the current
 * column address.
 */
int
MSIM_SH1106_SetColumn(struct MSIM_SH1106 *dev, uint8_t col)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-3))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = MSN(CMD_COLHADDR) | LSN(col >> 4);
		dev->buf[dev->blen+2] = CB_CMD;
		dev->buf[dev->blen+3] = MSN(CMD_COLLADDR) | LSN(col);
		dev->blen += 4;
	} else {
		rc = 1;
	}

	return rc;
}

/* Appends a command to the driver's buffer to switch the display on. */
int
MSIM_SH1106_DisplayOn(struct MSIM_SH1106 *dev)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = CMD_DISPLAYON;
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

/* Appends a command to the driver's buffer to switch the display off. */
int
MSIM_SH1106_DisplayOff(struct MSIM_SH1106 *dev)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = CMD_DISPLAYOFF;
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

int
MSIM_SH1106_SetContrast(struct MSIM_SH1106 *dev, uint8_t val)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-3))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = CMD_SETCONTRAST;
		dev->buf[dev->blen+2] = CB_CMD;
		dev->buf[dev->blen+3] = val;
		dev->blen += 4;
	} else {
		rc = 1;
	}

	return rc;
}

int
MSIM_SH1106_DisplayNormal(struct MSIM_SH1106 *dev)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = CMD_NORMALDISPLAY;
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

int
MSIM_SH1106_DisplayInvert(struct MSIM_SH1106 *dev)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = CMD_INVERTDISPLAY;
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

int
MSIM_SH1106_SetStartLine(struct MSIM_SH1106 *dev, uint8_t line)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = (uint8_t)
		        (CMD_SETSTARTLINE | ((uint8_t)(line & 0x3FU)));
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

int
MSIM_SH1106_SetScanDirection(struct MSIM_SH1106 *dev, uint8_t reverse)
{
	int rc = 0;

	WAIT_TILL_READY(dev);

	if ((dev != NULL) && (dev->blen < (BUFSZ-1))) {
		dev->buf[dev->blen] = CB_CMD;
		dev->buf[dev->blen+1] = (uint8_t)
		        (MSN(CMD_SETSCANDIR) | (uint8_t)((reverse << 3)&0x08));
		dev->blen += 2;
	} else {
		rc = 1;
	}

	return rc;
}

/*
 * Interrupt service routine for TWI interface.
 * It's used to send bytes of the driver's buffer to the display.
 */
ISR(TWI_vect)
{
	switch (TWI_STATUS) {
	/* --- Miscellaneous states --- */
	case START_SENT:
		if (cdev->state == CON_MASTER_TRANS) {
			/* Send SLA+W in this case */
			TWDR = (uint8_t) (cdev->addr << 1);
			RESUME_TRANS();
		}
		break;
	case RSTART_SENT:
		break;
	case ARB_LOST:
		if (cdev->state == CON_MASTER_TRANS) {
			cdev->state = CON_READY;
			cdev = NULL;
			SEND_STOP();
		}
		break;
	case NO_INFO:
		break;
	case BUS_ERROR:
		break;
	/* --- Master Transmitter states --- */
	case MT_SLAW_ACK:
		if (cdev->state == CON_MASTER_TRANS) {
			if (cdev->sent_i < cdev->sent_len) {
				/* Send one of the buffer bytes... */
				TWDR = cdev->buf[cdev->sent_i];
				cdev->sent_i++;
				RESUME_TRANS();
			} else {
				/* ... or STOP the transmission */
				cdev->state = CON_READY;
				cdev = NULL;
				SEND_STOP();
			}
		}
		break;
	case MT_SLAW_NACK:
		if (cdev->state == CON_MASTER_TRANS) {
			cdev->state = CON_READY;
			cdev = NULL;
			SEND_STOP();
		}
		break;
	case MT_DATA_ACK:
		if (cdev->state == CON_MASTER_TRANS) {
			if (cdev->sent_i < cdev->sent_len) {
				/* Send one of the buffer bytes... */
				TWDR = cdev->buf[cdev->sent_i];
				cdev->sent_i++;
				RESUME_TRANS();
			} else {
				/* ... or STOP the transmission */
				cdev->state = CON_READY;
				cdev = NULL;
				SEND_STOP();
			}
		}
		break;
	case MT_DATA_NACK:
		if (cdev->state == CON_MASTER_TRANS) {
			cdev->state = CON_READY;
			cdev = NULL;
			SEND_STOP();
		}
		break;
	default:
		break;
	}

	RESUME_TWI();
}

#endif /* defined(configMSIM_DRV_DISPLAY_SH1106_TWI) */
