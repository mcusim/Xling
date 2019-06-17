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
#ifndef XLING_OLED_SH1106_H_
#define XLING_OLED_SH1106_H_ 1

#include <stdint.h>

#include "xling/twi/twi.h"

#define SH1106_PLDSZ		256 /* Payload size, in bytes */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TWI control byte:
 *
 *     Co DC x x x x x x, where
 *
 *     Co = 0 - the last control byte, only data bytes follow,
 *     Co = 1 - the next two bytes are a data byte and another control byte,
 *     DC = 0 - the data byte is for command operation,
 *     DC = 1 - the data byte is for SRAM (framebuffer) operation.
 *
 */
enum SH1106_CB {
	CB_LASTCMD = 0x00,	/* 0 0 x x  x x x x */
	CB_LASTRAM = 0x40,	/* 0 1 x x  x x x x */
	CB_CMD = 0x80,		/* 1 0 x x  x x x x */
	CB_RAM = 0xC0,		/* 1 1 x x  x x x x */
};

struct SH1106 {
	struct TWI twi;			/* TWI interface to the display */
	uint8_t payload[SH1106_PLDSZ];	/* Commands, SRAM bytes, etc. */
	uint32_t plen;			/* Length of the payload, in bytes */
};

/* Utility functions */
void SH1106_TWIInit(struct SH1106 *dev);
void SH1106_Purge(struct SH1106 *dev);
void SH1106_Send(struct SH1106 *dev);
int SH1106_WriteControlByte(struct SH1106 *dev, enum SH1106_CB cb);
int SH1106_WriteDisplayData(struct SH1106 *dev, uint8_t byte);

/* Display commands */
int SH1106_SetPage(struct SH1106 *dev, uint8_t page);
int SH1106_SetColumn(struct SH1106 *dev, uint8_t col);
int SH1106_DisplayOn(struct SH1106 *dev);
int SH1106_DisplayOff(struct SH1106 *dev);
int SH1106_SetContrast(struct SH1106 *dev, uint8_t val);
int SH1106_DisplayNormal(struct SH1106 *dev);
int SH1106_DisplayReverse(struct SH1106 *dev);
int SH1106_SetStartLine(struct SH1106 *dev, uint8_t line);
int SH1106_SetScanDirection(struct SH1106 *dev, uint8_t direct);

/*
 * It doesn't implement a simple display command and should be a part of the
 * graphics/sh1106.c probably.
 */
int SH1106_Print(struct SH1106 *dev, const uint8_t *text);

#ifdef __cplusplus
}
#endif

#endif /* XLING_OLED_SH1106_H_ */
