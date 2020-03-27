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
#ifndef MSIM_DRV_DISPLAY_SH1106_H_
#define MSIM_DRV_DISPLAY_SH1106_H_ 1

/*
 * Low-level driver for SH1106-based displays.
 *
 * There are several options (pass to compiler as -Dxxx=YYY or -Dxxx)
 * to configure the driver:
 *
 *   configMSIM_DRV_DISPLAY_SH1106_DNUM
 *   ----------------------------------
 *
 *     Maximum number of the devices supported by the driver.
 *
 *     Memory for all of the display control blocks (DCBs) is statically
 *     allocated inside the driver, so select a minumum required number of
 *     the devices to minimize RAM footprint.
 *
 *   configMSIM_DRV_DISPLAY_SH1106_BUFSZ
 *   -----------------------------------
 *
 *     Defines size of the display buffer (in bytes) which is used to
 *     accumulate display commands and graphics RAM data.
 *
 *
 * These options are used to tell the driver how the display is connected:
 *
 *   configMSIM_DRV_DISPLAY_SH1106_TWIBB
 *   -----------------------------------
 *
 *     If defined, it tells the driver that display is connected
 *     via a software implemented TWI.
 *
 *   configMSIM_DRV_DISPLAY_SH1106_TWI
 *   ---------------------------------
 *
 *     If defined, it tells the driver that display is connected
 *     via a hardware implemented TWI.
 *
 *   configMSIM_DRV_DISPLAY_SH1106_SPI3
 *   ----------------------------------
 *
 *     NOTE: A 3-wire SPI mode isn't supported in case of the AVR MCU because
 *           of the 8-bit SPI shift register available in these devices only.
 *
 *           There is no room for the 9th bit required by the controller to
 *           distinct between data for graphics RAM (D/C bit = 1) and command
 *           register (D/C bit = 0).
 *
 *           Moreover, there is no easy way to stop transmitting bits from the
 *           shift register to "hack" the protocol.
 *
 *   configMSIM_DRV_DISPLAY_SH1106_SPI4
 *   ----------------------------------
 *
 *     If defined, it tells the driver that a display is connected
 *     via a 4-wire SPI.
 */

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes for the driver functions */
#define MSIM_SH1106_RC_OK			0
#define MSIM_SH1106_RC_NULLPTR			75
#define MSIM_SH1106_RC_DEVICENOTREADY		76
#define MSIM_SH1106_RC_DRIVERNOTREADY		77
#define MSIM_SH1106_RC_NOBUFSPACE		78
#define MSIM_SH1106_RC_WRONGDATTYPE		79
#define MSIM_SH1106_RC_UNKNOWNDATTYPE		80

/* The driver config */
typedef struct MSIM_SH1106DrvConf_t MSIM_SH1106DrvConf_t;

/* A display config */
typedef struct MSIM_SH1106Conf_t MSIM_SH1106Conf_t;

/* An opaque display control block (DCB) */
typedef struct MSIM_SH1106_t MSIM_SH1106_t;

/*
 * -----------------------------------------------------------------------------
 * An SH1106-based display can be connected by a software implemented
 * (bit banging) two-wire interface (I2C).
 * -----------------------------------------------------------------------------
 */
#if defined(configMSIM_DRV_DISPLAY_SH1106_TWIBB)

struct MSIM_SH1106Conf_t {
	volatile uint8_t *twi_port;	/* I/O port (PORTC, PORTD, etc.) */
	volatile uint8_t *twi_ddr;	/* DDR of the I/O port (DDRC, etc.) */
	uint8_t sda;			/* SDA pin number (PC0, PC1, etc.) */
	uint8_t scl;			/* SCL pin number (PC2, PC3, etc.) */
	uint8_t twi_addr;		/* TWI address of the display */
};

/*
 * Driver utility functions.
 *
 * NOTE: They aren't used in case of the software implemented TWI.
 */
#define MSIM_SH1106__drvStart(arg)
#define MSIM_SH1106__drvStop()

/*
 * -----------------------------------------------------------------------------
 * An SH1106-based display can be connected by a hardware implemented two-wire
 * interface (I2C).
 * -----------------------------------------------------------------------------
 */
#elif defined(configMSIM_DRV_DISPLAY_SH1106_TWI)

struct MSIM_SH1106DrvConf_t {
	uint32_t cpu_f;			/* CPU frequency, in Hz */
	uint32_t twi_f;			/* TWI (SCL) frequency, in Hz */
};

struct MSIM_SH1106Conf_t {
	uint8_t twi_addr;		/* TWI address of the display */
};

/*
 * Driver utility functions.
 *
 * NOTE: Don't forget to start the driver before making a call to any
 * driver's API functions.
 */
int	MSIM_SH1106__drvStart(const MSIM_SH1106DrvConf_t *);
int	MSIM_SH1106__drvStop(void);

/*
 * -----------------------------------------------------------------------------
 * An SH1106-based display can be connected by a 4-wire SPI.
 * -----------------------------------------------------------------------------
 */
#elif defined(configMSIM_DRV_DISPLAY_SH1106_SPI4)

struct MSIM_SH1106DrvConf_t {
	volatile uint8_t *port_spi;	/* I/O of the SPI port */
	volatile uint8_t *ddr_spi;	/* DDR of the SPI port */
	uint8_t mosi;			/* MOSI pin number */
	uint8_t miso;			/* MISO pin number */
	uint8_t sck;			/* SCK pin number */
};

struct MSIM_SH1106Conf_t {
	volatile uint8_t *rst_port;	/* I/O port for RST pin */
	volatile uint8_t *rst_ddr;	/* DDR of the port with RST pin */
	volatile uint8_t *cs_port;	/* I/O port for CS pin */
	volatile uint8_t *cs_ddr;	/* DDR of the port with CS pin */
	volatile uint8_t *dc_port;	/* I/O port of the D/C pin */
	volatile uint8_t *dc_ddr;	/* DDR of the the port with D/C pin */
	uint8_t rst;			/* Reset pin */
	uint8_t cs;			/* Chip-select pin */
	uint8_t dc;			/* Data/Command pin */
};

/*
 * Driver utility functions.
 *
 * NOTE: Don't forget to start the driver before making a call to any
 * driver's API functions.
 */
int	MSIM_SH1106__drvStart(const MSIM_SH1106DrvConf_t *);
int	MSIM_SH1106__drvStop(void);

/* -------------------------------------------------------------------------- */
#else
#error "Let the driver know how the SH1106-based display is connected!"
#endif

/*
 * API functions of the driver.
 */

/* DCB utility functions */
MSIM_SH1106_t *	MSIM_SH1106_Init(const MSIM_SH1106Conf_t *);
void		MSIM_SH1106_Free(MSIM_SH1106_t *);
void		MSIM_SH1106_Wait(MSIM_SH1106_t *);

/* DCB's buffer utility functions */
int	MSIM_SH1106_bufClear(MSIM_SH1106_t *);
int	MSIM_SH1106_bufSend(MSIM_SH1106_t *);
int	MSIM_SH1106_bufAppend(MSIM_SH1106_t *, const uint8_t);
int	MSIM_SH1106_bufAppendLast(MSIM_SH1106_t *, const uint8_t *, size_t);
int	MSIM_SH1106_bufAppendLast_PF(MSIM_SH1106_t *, const uint8_t *, size_t);

/* Display commands */
int	MSIM_SH1106_SetPage(MSIM_SH1106_t *, uint8_t page);
int	MSIM_SH1106_SetColumn(MSIM_SH1106_t *, uint8_t col);
int	MSIM_SH1106_DisplayOn(MSIM_SH1106_t *);
int	MSIM_SH1106_DisplayOff(MSIM_SH1106_t *);
int	MSIM_SH1106_SetContrast(MSIM_SH1106_t *, uint8_t val);
int	MSIM_SH1106_DisplayNormal(MSIM_SH1106_t *);
int	MSIM_SH1106_DisplayInvert(MSIM_SH1106_t *);
int	MSIM_SH1106_SetStartLine(MSIM_SH1106_t *, uint8_t line);
int	MSIM_SH1106_SetScanDirection(MSIM_SH1106_t *, uint8_t reverse);

#ifdef __cplusplus
}
#endif

#endif /* MSIM_DRV_DISPLAY_SH1106_H_ */
