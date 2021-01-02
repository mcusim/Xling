/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of a firmware for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2019, 2020 Dmitry Salychev
 *
 * Xling firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Xling firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "xling/graphics.h"

#define CHAR_WIDTH	3 /* bits */
#define PAGE_HEIGHT	8 /* bits */
#define PAGES		8 /* in graphic RAM */
#define NOT(u8)		((uint8_t)(~(u8)))
#define PGM(a)		((uint8_t)(pgm_read_byte_far((a))))


static void     get_img_byte(const XG_Image_t * const image,
                    const uint32_t idx, uint8_t *data, uint8_t *alpha);

/*
 * Prints text on the canvas at the given coordinates.
 */
int
XG_Print(XG_Canvas_t *canvas, const XG_Text_t *text, XG_Point_t pt)
{
	const size_t len = strlen(text->text);
	const XG_Font_t *font = text->font;
	XG_Image_t image;
	char ch;
	int rc;

	/* Draw characters on the canvas. */
	for (uint32_t i = 0; i < len; i++) {
		/* Obtain the current character. */
		ch = text->text[i];

		/* Obtain image of the current glyph. */
		image.data = font->glyphs[ch-0x20].data;
		image.alpha = NULL;
		image.width = font->glyphs[ch-0x20].width;
		image.height = font->glyphs[ch-0x20].height;
		image.data_size = font->glyphs[ch-0x20].data_size;

		rc = XG_Draw_PF(canvas, &image, pt);
		pt.x += image.width;

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
XG_Draw_PF(XG_Canvas_t *canvas, const XG_Image_t *image, XG_Point_t pt)
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
	const uint16_t x = pt.x;
	const uint16_t y = pt.y;

	/* Image size */
	const uint16_t iw = ((x + image->width) > canvas->width)
	        ? image->width - ((x + image->width) - canvas->width)
	        : image->width;
	const uint16_t ih = ((y + image->height) > canvas->height)
	        ? image->height - ((y + image->height) - canvas->height)
	        : image->height;

	/* Display pages */
	const uint16_t start_page = y / PAGE_HEIGHT;
	uint16_t end_page = (y + ih + PAGE_HEIGHT - 1) / PAGE_HEIGHT;

	/* Shifts */
	const uint8_t shift = y % PAGE_HEIGHT;
	const uint8_t bottom_shift = (((y + ih) % PAGE_HEIGHT) != 0)
	        ? (uint8_t)(PAGE_HEIGHT - ((y + ih) % PAGE_HEIGHT)) : 0u;

	/* Masks */
	const uint8_t mask = (uint8_t)(0xFFu << shift);
	const uint8_t bottom_mask = (uint8_t)(0xFFu >> bottom_shift);
	const uint8_t img_mask = (uint8_t)(mask & bottom_mask);

	uint16_t canvas_idx, img_idx = 0;
	uint8_t top_byte, img_byte, bottom_byte;
	uint8_t raw_byte = 0, alpha_mask = 0xFF;
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
			/*
			 * 1. Data byte at the "top" of the current one.
			 */
			if (i == start_page) {
				top_byte = (canvas->data[j] & NOT(mask));
			} else {
				/*
				 * Obtain a byte of image data and its alpha
				 * channel (or 0xFF if no alpha channel).
				 */
				get_img_byte(image, img_idx - image->width,
				    &raw_byte, &alpha_mask);

				/* With alpha channel */
				top_byte  = canvas->data[j]
				    & NOT(mask)
				    & NOT((alpha_mask >> (PAGE_HEIGHT - shift))
					& NOT(mask));
				top_byte |= (uint8_t)((raw_byte & alpha_mask) >>
				    (PAGE_HEIGHT - shift));
			}

			/*
			 * 2. Byte with the current image data.
			 */
			if (i == (end_page - 1)) {
				/* Image data at the end of the image. */
				get_img_byte(image, img_idx, &raw_byte,
				    &alpha_mask);

				/* With alpha channel */
				img_byte = canvas->data[j]
				    & mask
				    & NOT((alpha_mask << shift) & mask);
				img_byte = (uint8_t)(img_byte | ((img_mask != 0)
				    ? ((raw_byte & alpha_mask) << shift)
				    : 0));
			} else {
				/* Image data somewhere before end. */
				get_img_byte(image, img_idx, &raw_byte,
				    &alpha_mask);

				/* With alpha channel */
				img_byte = canvas->data[j]
				    & mask
				    & NOT((alpha_mask << shift) & mask);
				img_byte |= (uint8_t)((raw_byte & alpha_mask) <<
				    shift);
			}

			/*
			 * 3. Data byte at the "bottom" of the current one.
			 */
			if (i == (end_page - 1)) {
				bottom_byte = (uint8_t)
				    (canvas->data[j] & NOT(bottom_mask));
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

void
XG_TransferCanvas(MSIM_SH1106_t *display, const XG_Canvas_t *canvas)
{
	for (uint32_t i = 0; i < 8; i++) {
		MSIM_SH1106_bufClear(display);
		MSIM_SH1106_SetPage(display, (uint8_t) i);
		MSIM_SH1106_SetColumn(display, 2);
		MSIM_SH1106_bufSend(display);

		MSIM_SH1106_bufClear(display);
		MSIM_SH1106_bufAppendLast(
		    display, &(canvas->data[i * 128]), 128);
		MSIM_SH1106_bufSend(display);
	}
}

int
XG_DrawScene(XG_Canvas_t *canvas, const XG_Scene_t *scene)
{
	const XG_Layer_t *layer;
	const XG_Image_t *img;
	XG_Animation_t *anim;
	const XG_AnimFrame_t *frames;
	const XG_AnimFrame_t *frame;
	uint16_t rnd;

	for (uint16_t i = scene->layers_n; i > 0; i--) {
		layer = &scene->layers[i - 1];

		switch (layer->obj_type) {
		case XG_OT_Image:
			/* Let's draw a static image. */
			img = (const XG_Image_t *) layer->obj;
			XG_Draw_PF(canvas, img, layer->base_pt);

			break;
		case XG_OT_Animation:
			/* Let's draw an animation. */
			anim = (XG_Animation_t *) layer->obj;
			frames = (const XG_AnimFrame_t *) anim->frames;
			frame = &frames[anim->frame_idx];

			/* Don't draw an inactive animation. */
			if (anim->active == 0) {
				break;
			}

			/* Draw the current frame. */
			XG_Draw_PF(canvas, frame->img, frame->base_pt);

			/* Choose the next frame index. */
			if (anim->stay_cnt == 0u) {
				if (frame->alt_chance > 0) {
					rnd = (uint16_t)
					    (1 + rand() / ((RAND_MAX + 1u) / 100));

					if (rnd <= frame->alt_chance) {
						anim->frame_idx = frame->alt;
					} else {
						anim->frame_idx++;
					}
					anim->frame_idx =
					    anim->frame_idx >= anim->frames_n
					    ? 0 : anim->frame_idx;

					/* Update stay counter. */
					anim->stay_cnt =
					    frames[anim->frame_idx].stay;
				} else {
					/* No chance to use an alternative frame. */
					anim->frame_idx++;
					anim->frame_idx =
					    anim->frame_idx >= anim->frames_n
					    ? 0 : anim->frame_idx;

					/* Update stay counter. */
					anim->stay_cnt =
					    frames[anim->frame_idx].stay;
				}
			} else {
				anim->stay_cnt--;
			}

			break;
		default:
			/* Other object types can't be painted at the moment. */
			break;
		}
	}

	return 0;
}

static void
get_img_byte(const XG_Image_t * const image, const uint32_t idx,
    uint8_t *data, uint8_t *alpha)
{
	if (image->alpha == NULL) {
		/* Simple monochrome image. */
		(*data) = PGM(&image->data[idx]);
		(*alpha) = 0xFF;
	} else {
		/* Monochrome image with 1-bit alpha channel. */
		(*data) = PGM(&image->data[idx]);
		(*alpha) = PGM(&image->alpha[idx]);
	}
}
