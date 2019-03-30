/*
 * Copyright (c) 2017, 2018, The MCUSim Contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the MCUSim or its parts nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
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
 * Functions to work with SSD1309-based display.
 */
#include <stdint.h>
#include <string.h>

#include "xling/twi.h"
#include "xling/font.h"
#include "xling/hw/ssd1309.h"

/* Fundamental Command Table */
#define CMD_SETCONTRAST			0x81
#define CMD_DISPLAYALLON_RESUME		0xA4
#define CMD_DISPLAYALLON		0xA5
#define CMD_NORMALDISPLAY		0xA6
#define CMD_INVERTDISPLAY		0xA7
#define CMD_DISPLAYOFF			0xAE
#define CMD_DISPLAYON			0xAF
#define CMD_NOP				0xE3
#define CMD_CMDLOCK			0xFD

/* Addressing Setting Command Table */
#define CMD_MEMORYMODE			0x20
#define CMD_COLUMNADDR			0x21
#define CMD_PAGEADDR			0x22

/* Hardware Configuration Command Table */
#define CMD_SEGREMAP			0xA0
#define CMD_INVERTSEGREMAP		0xA1
#define CMD_SETMULTIPLEX		0xA8
#define CMD_COMSCANINC			0xC0
#define CMD_COMSCANDEC			0xC8
#define CMD_SETVERTSHIFT		0xD3
#define CMD_SETCOMPINS			0xDA
#define CMD_SETGPIO			0xDC

/* Other commands */
#define CMD_SETVCOMDETECT		0xDB
#define CMD_SETDISPLAYCLOCKDIV		0xD5
#define CMD_SETPRECHARGE		0xD9
#define CMD_SETLOWCOLUMN		0x00
#define CMD_SETHIGHCOLUMN		0x10
#define CMD_SETSTARTLINE		0x40
#define CMD_CHARGEPUMP			0x8D
#define CMD_SWITCHCAPVCC		0x02

static void send_command(struct XG_SSD1309 *dev, uint8_t cmd);

void XG_SSD1309TWIInit(struct XG_TWI *twi, struct XG_SSD1309 *dev)
{
	dev->twi = twi;

	send_command(dev, CMD_DISPLAYOFF);
	send_command(dev, CMD_SETDISPLAYCLOCKDIV);
	send_command(dev, 0xF0);
	send_command(dev, CMD_SETVERTSHIFT);
	send_command(dev, 0x00);
	send_command(dev, CMD_SETSTARTLINE | 0x00);
	/* Horizontal memory mode */
	send_command(dev, CMD_MEMORYMODE);
	send_command(dev, 0x00);
	send_command(dev, CMD_SETCONTRAST);
	send_command(dev, 0x10);
	/* Non-inverted display */
	send_command(dev, CMD_NORMALDISPLAY);
	send_command(dev, CMD_DISPLAYON);
}

void XG_SSD1309Print(struct XG_SSD1309 *dev, const uint8_t *text)
{
	struct XG_TWI *twi = dev->twi;
	size_t len = strlen((const char *)text);

	XG_TWIStart(twi);
	XG_TWIWrite(twi, 0x40);
	for (uint32_t i = 0; i < len; i++) {
		switch (text[i]) {
		case '0':
			XG_TWIWriteData(twi, mark_zero, sizeof mark_zero);
			break;
		case '1':
			XG_TWIWriteData(twi, mark_one, sizeof mark_one);
			break;
		case '2':
			XG_TWIWriteData(twi, mark_two, sizeof mark_two);
			break;
		case '3':
			XG_TWIWriteData(twi, mark_three, sizeof mark_three);
			break;
		case '4':
			XG_TWIWriteData(twi, mark_four, sizeof mark_four);
			break;
		case '5':
			XG_TWIWriteData(twi, mark_five, sizeof mark_five);
			break;
		case '6':
			XG_TWIWriteData(twi, mark_six, sizeof mark_six);
			break;
		case '7':
			XG_TWIWriteData(twi, mark_seven, sizeof mark_seven);
			break;
		case '8':
			XG_TWIWriteData(twi, mark_eight, sizeof mark_eight);
			break;
		case '9':
			XG_TWIWriteData(twi, mark_nine, sizeof mark_nine);
			break;
		case '.':
			XG_TWIWriteData(twi, mark_dot, sizeof mark_dot);
			break;
		default:
			break;
		}
	}
	XG_TWIStop(twi);
}

void XG_SSD1309SetPage(struct XG_SSD1309 *dev, uint8_t page)
{
	send_command(dev, CMD_PAGEADDR);
	send_command(dev, page);
	send_command(dev, 0x07);
}

void XG_SSD1309SetColumn(struct XG_SSD1309 *dev, uint8_t col)
{
	send_command(dev, CMD_COLUMNADDR);
	send_command(dev, col);
	send_command(dev, 0x7F);
}

static void send_command(struct XG_SSD1309 *dev, uint8_t cmd)
{
	XG_TWIStart(dev->twi);
	XG_TWIWrite(dev->twi, 0x00);
	XG_TWIWrite(dev->twi, cmd);
	XG_TWIStop(dev->twi);
}
