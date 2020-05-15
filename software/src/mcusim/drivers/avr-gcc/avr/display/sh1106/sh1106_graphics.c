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
#include <avr/pgmspace.h>

/*
 * Graphics for SH1106-based displays.
 */

#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106_graphics.h"
#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106_fonts.h"

/* Local macros */
#define CHAR_WIDTH	3 /* bits */
#define PAGE_HEIGHT	8 /* bits */
#define PAGES		8 /* in graphic RAM */
#define FONT_3x6	MSIM_SH1106_FONT_TOMTHUMB_3x6
#define NOT(u8)		((uint8_t)(~(u8)))
#define PGM(a)		((uint8_t)(pgm_read_byte_far((a))))

/* Local functions */
static void	write_mem(MSIM_SH1106_t *, const uint8_t *, size_t);

/* Prints text at the current line of the display. */
int
MSIM_SH1106_Print(MSIM_SH1106_t *dev, const char *text)
{
	const size_t len = strlen(text);

	MSIM_SH1106_bufClear(dev);

	/* Append characters to the display buffer. */
	for (uint32_t i = 0; i < len; i++) {
		MSIM_SH1106_bufAppend(dev, 0);
		write_mem(dev, &FONT_3x6[text[i] * CHAR_WIDTH], CHAR_WIDTH);
	}

	MSIM_SH1106_bufSend(dev);

	return 0;
}

/*
 * Draws an image on a canvas at the given coordinates.
 *
 * NOTE: Image data should be located in flash memory and will be accessed by
 *       a far (32-bit) pointer. Canvas data will be accessed directly.
 */
int
MSIM_SH1106_Draw_PF(MSIM_SH1106Canvas_t * const canvas,
                    const MSIM_SH1106Image_t * const image,
                    const uint16_t x, const uint16_t y)
{
	/* Re-calculate image size to draw a visible part of the image only. */
	const uint16_t iw = ((x + image->width) > canvas->width)
	        ? image->width - ((x + image->width) - canvas->width)
	        : image->width;
	const uint16_t ih = ((y + image->height) > canvas->height)
	        ? image->height - ((y + image->height) - canvas->height)
	        : image->height;
	/* Canvas's pages to modify. */
	const uint16_t start_page = y / PAGE_HEIGHT;
	uint16_t end_page = (y + ih + PAGE_HEIGHT - 1) / PAGE_HEIGHT;
	/*
	 * Shifts (in pixels) from the top border of the start page and
	 * from the bottom border of the end page.
	 */
	const uint8_t shift = y % PAGE_HEIGHT;
	const uint8_t bottom_shift = (((y + ih) % PAGE_HEIGHT) != 0)
	        ? (uint8_t)(PAGE_HEIGHT - ((y + ih) % PAGE_HEIGHT)) : 0u;
	/* Pixel masks. */
	const uint8_t mask = (uint8_t)(0xFFu << shift);
	const uint8_t bottom_mask = (uint8_t)(0xFFu >> bottom_shift);
	const uint8_t img_mask = (uint8_t)(mask & bottom_mask);

	uint16_t canvas_idx, img_idx = 0;
	uint8_t top_byte, img_byte, bottom_byte;
	int rc = 0;

	/* Check indexes of the pages. */
	if ((start_page > (PAGES - 1)) || (end_page > PAGES)) {
		/* Do not draw anything. */
		end_page = 0;
		rc = 1;
	}
	/* Check coordinates. */
	if ((x > (canvas->width - 1)) || (y > (canvas->height - 1))) {
		/* Do not draw anything. */
		end_page = 0;
		rc = 1;
	}

	/* An image drawing loop. */
	for (uint16_t i = start_page; i < end_page; i++) {
		/* Calculate index for the current line in canvas. */
		canvas_idx = (i * canvas->width) + x;

		/* Draw the current line. */
		for (uint16_t j = canvas_idx; j < (canvas_idx + iw); j++) {
			/* Data byte at the "top" of the current one. */
			if (i == start_page) {
				top_byte = (canvas->data[j] & NOT(mask));
			} else {
				top_byte = PGM(
				        &image->data[img_idx - image->width]);
				top_byte = (uint8_t)
				        (top_byte >> (PAGE_HEIGHT - shift));
			}

			/* Byte with current image data. */
			if (i == (end_page - 1)) {
				img_byte = (uint8_t)((img_mask != 0)
				        ? (PGM(&image->data[img_idx]) << shift)
					: 0);
			} else {
				img_byte = (uint8_t)(
				        PGM(&image->data[img_idx]) << shift);
			}

			/* Data byte at the "bottom" of the current one. */
			if (i == (end_page - 1)) {
				bottom_byte = (uint8_t)(
				        canvas->data[j] & NOT(bottom_mask));
			} else {
				bottom_byte = 0;
			}

			/* Draw the actual byte on the canvas. */
			canvas->data[j] = top_byte | img_byte | bottom_byte;

			/* Increment image index. */
			img_idx++;
		}

		/* Move to the "next line" on the image. */
		img_idx += image->width - iw;
	}

	return rc;
}

static void
write_mem(MSIM_SH1106_t *dev, const uint8_t *data, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		MSIM_SH1106_bufAppend(dev, pgm_read_byte(&data[i]));
	}
}
