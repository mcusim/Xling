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

/* Local macros */
#define CHAR_WIDTH	3 /* bits */
#define PAGE_HEIGHT	8 /* bits */
#define PAGES		8 /* in graphic RAM */
#define NOT(u8)		((uint8_t)(~(u8)))
#define PGM(a)		((uint8_t)(pgm_read_byte_far((a))))

/*
 * Prints text on the canvas at the given coordinates.
 */
int
MSIM_SH1106_Print(MSIM_SH1106Canvas_t * const canvas,
                  MSIM_SH1106Text_t * const text,
                  const uint16_t x, const uint16_t y)
{
	const size_t len = strlen(text->text);
	const MSIM_SH1106Font_t *font = text->font;
	MSIM_SH1106Image_t image;
	uint16_t x_advance = 0u;
	char ch;
	int rc;

	/* Draw characters on the canvas. */
	for (uint32_t i = 0; i < len; i++) {
		/* Obtain the current character. */
		ch = text->text[i];

		/*
		 * Obtain image of the current glyph.
		 */
		image.data = font->glyphs[ch-0x20].data;
		image.width = font->glyphs[ch-0x20].width;
		image.height = font->glyphs[ch-0x20].height;
		image.data_size = font->glyphs[ch-0x20].data_size;

		rc = MSIM_SH1106_Draw_PF(canvas, &image, x + x_advance, y);

		x_advance += image.width;

		if (rc != 0) {
			break;
		}
	}

	return rc;
}

/*
 * Draws an image on a canvas at the given coordinates.
 *
 * NOTE: Image data should be located in the flash memory and will be accessed
 *       by a far (32-bit) pointer. Canvas data will be accessed directly.
 */
int
MSIM_SH1106_Draw_PF(MSIM_SH1106Canvas_t * const canvas,
                    const MSIM_SH1106Image_t * const image,
                    const uint16_t x, const uint16_t y)
{
	/*
	 * Local read-only variables:
	 *
	 * iw, ih:
	 *
	 *     Re-calculated image width and height to draw a visible part of
	 *     the image only.
	 *
	 * start_page, end_page:
	 *
	 *     Pages of the canvas which will be modified during an image
	 *     drawing process.
	 *
	 * shift, bottom_shift:
	 *
	 *     Shifts (in pixels) from the top border of the start page and
	 *     from the bottom border of the end page.
	 *
	 * mask, bottom_mask, img_mask:
	 *
	 *     Pixel masks.
	 */

	const uint16_t iw = ((x + image->width) > canvas->width)
	        ? image->width - ((x + image->width) - canvas->width)
	        : image->width;
	const uint16_t ih = ((y + image->height) > canvas->height)
	        ? image->height - ((y + image->height) - canvas->height)
	        : image->height;
	const uint16_t start_page = y / PAGE_HEIGHT;
	uint16_t end_page = (y + ih + PAGE_HEIGHT - 1) / PAGE_HEIGHT;
	const uint8_t shift = y % PAGE_HEIGHT;
	const uint8_t bottom_shift = (((y + ih) % PAGE_HEIGHT) != 0)
	        ? (uint8_t)(PAGE_HEIGHT - ((y + ih) % PAGE_HEIGHT)) : 0u;
	const uint8_t mask = (uint8_t)(0xFFu << shift);
	const uint8_t bottom_mask = (uint8_t)(0xFFu >> bottom_shift);
	const uint8_t img_mask = (uint8_t)(mask & bottom_mask);

	uint16_t canvas_idx, img_idx = 0;
	uint8_t top_byte, img_byte, bottom_byte;
	int rc = 0;

	/* Check indexes of the pages. */
	if ((start_page > (PAGES - 1)) || (end_page > PAGES)) {
		end_page = 0;
		rc = 1;
	}
	/* Check coordinates. */
	if ((x > (canvas->width - 1)) || (y > (canvas->height - 1))) {
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
