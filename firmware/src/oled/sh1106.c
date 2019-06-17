/*
 * Copyright 2017-2019 The MCUSim Project.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the MCUSim or its parts nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Low-level utility functions and commands for SH1106-based OLED display.
 */
#include <stdint.h>
#include <string.h>

#include "xling/graphics/font.h"
#include "xling/oled/sh1106.h"
#include "xling/twi/twi.h"

/* Fundamental Command Table */
#define CMD_DISPLAYOFF			0xAE
#define CMD_DISPLAYON			0xAF
#define CMD_SETCONTRAST			0x81
#define CMD_NORMALDISPLAY		0xA6
#define CMD_INVERTDISPLAY		0xA7
#define CMD_SETSTARTLINE		0x40 /* Prefix for 0x40-0x7F commands */
#define CMD_PAGEADDR			0xB0 /* Prefix for 0xB0-0xB7 commands */
#define CMD_COLHADDR			0x10
#define CMD_COLLADDR			0x00
#define CMD_SETSCANDIR			0xC0

#define MSB(v)		((uint8_t) (((uint8_t)(v)) & 0xF0U))
#define LSB(v)		((uint8_t) (((uint8_t)(v)) & 0x0FU))

/* Initialize an SH1106-based OLED display connected using TWI. */
void
SH1106_TWIInit(struct SH1106 *dev)
{
	TWI_Init(&dev->twi);

	SH1106_Purge(dev);
	SH1106_DisplayOff(dev);
	SH1106_SetContrast(dev, 0x57);
	SH1106_DisplayNormal(dev);
	SH1106_DisplayOn(dev);
	SH1106_Send(dev);
}

/* Clean a payload buffer. */
void
SH1106_Purge(struct SH1106 *dev)
{
	dev->plen = 0;
}

/* Send a payload buffer with commands and/or SRAM data. */
void
SH1106_Send(struct SH1106 *dev)
{
	const uint32_t len = SH1106_PLDSZ < dev->plen
		? SH1106_PLDSZ : dev->plen;

	TWI_Start(&dev->twi);
	TWI_WriteData(&dev->twi, dev->payload, len);
	TWI_Stop(&dev->twi);
}

/* Append control byte to the payload buffer. */
int
SH1106_WriteControlByte(struct SH1106 *dev, enum SH1106_CB cb)
{
	if (dev->plen < SH1106_PLDSZ) {
		dev->payload[dev->plen] = (uint8_t)cb;
		dev->plen++;
	}
	return 0;
}

/*
 * Append SRAM byte to the payload buffer.
 * NOTE: Column address will be incremented, but page address - won't be.
 */
int
SH1106_WriteDisplayData(struct SH1106 *dev, uint8_t byte)
{
	if (dev->plen < SH1106_PLDSZ) {
		dev->payload[dev->plen] = byte;
		dev->plen++;
	}
	return 0;
}

/* Append a command to change the current page address. */
int
SH1106_SetPage(struct SH1106 *dev, uint8_t page)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = MSB(CMD_PAGEADDR) | (page & 0x07U);
		dev->plen += 2;
	}
	return 0;
}

/* Append a command to change the current column address. */
int
SH1106_SetColumn(struct SH1106 *dev, uint8_t col)
{
	if (dev->plen < (SH1106_PLDSZ-3)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = MSB(CMD_COLHADDR) | LSB(col>>8);
		dev->payload[dev->plen+2] = CB_CMD;
		dev->payload[dev->plen+3] = MSB(CMD_COLLADDR) | LSB(col);
		dev->plen += 4;
	}
	return 0;
}

/* Turns an OLED display on. */
int
SH1106_DisplayOn(struct SH1106 *dev)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = CMD_DISPLAYON;
		dev->plen += 2;
	}
	return 0;
}

/* Turns an OLED display off. */
int
SH1106_DisplayOff(struct SH1106 *dev)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = CMD_DISPLAYOFF;
		dev->plen += 2;
	}
	return 0;
}

int
SH1106_SetContrast(struct SH1106 *dev, uint8_t val)
{
	if (dev->plen < (SH1106_PLDSZ-3)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = CMD_SETCONTRAST;
		dev->payload[dev->plen+2] = CB_CMD;
		dev->payload[dev->plen+3] = val;
		dev->plen += 4;
	}
	return 0;
}

int
SH1106_DisplayNormal(struct SH1106 *dev)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = CMD_NORMALDISPLAY;
		dev->plen += 2;
	}
	return 0;
}

int
SH1106_DisplayReverse(struct SH1106 *dev)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = CMD_INVERTDISPLAY;
		dev->plen += 2;
	}
	return 0;
}

int
SH1106_SetStartLine(struct SH1106 *dev, uint8_t l)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] = CMD_SETSTARTLINE | (l & 0x3FU);
		dev->plen += 2;
	}
	return 0;
}

int
SH1106_SetScanDirection(struct SH1106 *dev, uint8_t reverse)
{
	if (dev->plen < (SH1106_PLDSZ-1)) {
		dev->payload[dev->plen] = CB_CMD;
		dev->payload[dev->plen+1] =
			MSB(CMD_SETSCANDIR) | ((reverse<<3)&0x08U);
		dev->plen += 2;
	}
	return 0;
}

/* Print text at the first line of the display. */
int
SH1106_Print(struct SH1106 *dev, const uint8_t *text)
{
	struct TWI *twi = &dev->twi;
	size_t len = strlen((const char *)text);

	TWI_Start(twi);
	TWI_Write(twi, CB_LASTRAM);

	for (uint32_t i = 0; i < len; i++) {
		switch (text[i]) {
		case '0':
			TWI_WriteData(twi, mark_zero, sizeof mark_zero);
			break;
		case '1':
			TWI_WriteData(twi, mark_one, sizeof mark_one);
			break;
		case '2':
			TWI_WriteData(twi, mark_two, sizeof mark_two);
			break;
		case '3':
			TWI_WriteData(twi, mark_three, sizeof mark_three);
			break;
		case '4':
			TWI_WriteData(twi, mark_four, sizeof mark_four);
			break;
		case '5':
			TWI_WriteData(twi, mark_five, sizeof mark_five);
			break;
		case '6':
			TWI_WriteData(twi, mark_six, sizeof mark_six);
			break;
		case '7':
			TWI_WriteData(twi, mark_seven, sizeof mark_seven);
			break;
		case '8':
			TWI_WriteData(twi, mark_eight, sizeof mark_eight);
			break;
		case '9':
			TWI_WriteData(twi, mark_nine, sizeof mark_nine);
			break;
		case '.':
			TWI_WriteData(twi, mark_dot, sizeof mark_dot);
			break;
		default:
			break;
		}
	}
	TWI_Stop(twi);

	return 0;
}
