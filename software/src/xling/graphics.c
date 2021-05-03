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

#define CHAR_WIDTH		3 /* bits */
#define PHEIGHT			8 /* bits */
#define PAGES			8 /* in graphic RAM */
#define MAX_CACHED_LAYERS	64
#define SPEECH_DELAY_CYCLES	(1u)
#define LINE_HEIGHT		(12u) /* px */
#define NOT(u8)			((uint8_t)(~(u8)))
#define PGM(a)			((uint8_t)(pgm_read_byte_far((a))))

typedef enum {
	CACHE_INVALID = 0,
	CACHE_VALID
} cache_state_e;

static void	get_img_byte(const xg_image_t * const image,
    const uint32_t idx, uint8_t *data, uint8_t *alpha);
static void	copy_canvas(xg_canvas_t *dest, const xg_canvas_t *src);

/*
 * Calculates an auxiliary point with non-negative coordinates (__x, __y) which
 * will be used as a new top-left corner to draw a visible part of the orignal
 * image only. In addition, offset (__idx) in the image data array, (__width)
 * and (__height) of the new image will be provided.
 */
#define CALC_AUX_POINT(__pt, __image, __x, __y, __idx, __width, __height) do { \
	const uint16_t __off = (uint16_t)abs((__pt)->y % PHEIGHT);	\
	int16_t __sw = (int16_t)(__image)->width; /* signed width */	\
	int16_t __sh = (int16_t)(__image)->height; /* signed height */	\
	int16_t __sx, __sy; /* signed coordinates */			\
									\
	(__x) = ((__pt)->x < 0) ? 0u : (uint16_t)(__pt)->x;		\
	(__y) = ((__pt)->y < 0) ? (__off != 0u && abs((__pt)->y) < __sh) \
	    ? (uint16_t)(PHEIGHT - __off) : 0u				\
	    : (uint16_t)(__pt)->y;					\
	__sx = (int16_t)(__x);						\
	__sy = (int16_t)(__y);						\
									\
	(__idx) = ((__pt)->x >= 0 && (__pt)->y >= 0) ? 0u		\
	    : (uint16_t)((((__sy - (__pt)->y) / PHEIGHT) * __sw) +	\
		((__pt)->x < 0 ? abs((__pt)->x) : 0));			\
									\
	(__width)  = (__sw - (__sx - (__pt)->x)) < 0 ? 0u		\
	    : (uint16_t)(__sw - (__sx - (__pt)->x));			\
	(__height) = (__sh - (__sy - (__pt)->y)) < 0 ? 0u		\
	    : (uint16_t)(__sh - (__sy - (__pt)->y));			\
} while (0)

static xg_canvas_t *cache_canvas = NULL;
static xg_point_t cache_pts[MAX_CACHED_LAYERS];
static uint8_t cached_layers = 0;
static cache_state_e cache_state = CACHE_INVALID;

/* Prints text on the canvas at the given coordinates. */
int
xg_print(xg_canvas_t *canvas, const xg_text_t *text, xg_point_t pt)
{
	const size_t len = strlen(text->text);
	const xg_font_t *font = text->font;
	xg_image_t glyph;
	int glyph_idx, rc;

	/* Draw characters on the canvas. */
	for (uint32_t i = 0; i < len; i++) {
		/* Obtain an index of the current glyph. */
		glyph_idx = text->text[i] - 0x20;

		/* Obtain the current glyph. */
		glyph.alpha = NULL;
		glyph.data = font->glyphs[glyph_idx].data;
		glyph.width = font->glyphs[glyph_idx].width;
		glyph.height = font->glyphs[glyph_idx].height;
		glyph.data_size = font->glyphs[glyph_idx].data_size;

		/* Draw the glyph. */
		rc = xg_draw_pf(canvas, &glyph, pt);
		if (rc != 0) {
			break;
		}

		/* Move the text cursor forward. */
		pt.x += glyph.width > INT16_MAX ? 0 : (int16_t) glyph.width;
	}

	return rc;
}

int
xg_draw_speech(xg_canvas_t *canvas, xg_text_t *text)
{
	const xg_font_t *font = text->font;
	const size_t text_len = strlen(text->text);
	size_t drawn_len = text->drawn_len;
	xg_point_t drawn_pt = text->drawn_pt;
	xg_image_t glyph;
	int glyph_idx;
	int rc = XG_SPM_MORE;

	if (drawn_len >= text_len) {
		rc = XG_SPM_STOP;
	}

	if (text->skip_cycles == SPEECH_DELAY_CYCLES && rc != XG_SPM_STOP) {
		text->skip_cycles = 0;

		/* Obtain an index of the current glyph. */
		glyph_idx = text->text[drawn_len] - 0x20;

		/* Obtain the current glyph. */
		glyph.alpha = NULL;
		glyph.data = font->glyphs[glyph_idx].data;
		glyph.width = font->glyphs[glyph_idx].width;
		glyph.height = font->glyphs[glyph_idx].height;
		glyph.data_size = font->glyphs[glyph_idx].data_size;

		/* Move cursor to a new line if needed. */
		if (drawn_pt.x + (glyph.width > INT16_MAX
		    ? 0 : (int16_t) glyph.width) > canvas->width) {
			drawn_pt.x = 0;
			drawn_pt.y += LINE_HEIGHT;
		}

		/* Draw the glyph. */
		rc = xg_draw_pf(canvas, &glyph, drawn_pt);
		if (rc != 0) {
			/* break; */
		}

		/* Move the text cursor forward. */
		drawn_pt.x += glyph.width > INT16_MAX
		    ? 0 : (int16_t) glyph.width;
		drawn_len++;

		text->drawn_pt = drawn_pt;
		text->drawn_len = drawn_len;
	} else if (rc != XG_SPM_STOP) {
		text->skip_cycles++;
	} else {
		/* Nothing to do here. */
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
xg_draw_pf(xg_canvas_t *canvas, const xg_image_t *image, xg_point_t pt)
{
	uint16_t x, y;
	uint16_t iw, ih;
	uint16_t canvas_idx, img_idx;
	uint8_t top_byte, img_byte, bottom_byte;
	uint8_t raw_byte = 0, alpha_mask = 0xFF;
	int rc = 0;

	CALC_AUX_POINT(&pt, image, x, y, img_idx, iw, ih);

	/* Size of the visible image part */
	iw = ((x + iw) > canvas->width) ? iw - ((x + iw) - canvas->width) : iw;
	ih = ((y + ih) > canvas->height) ? ih - ((y + ih) - canvas->height) : ih;

	/* Canvas pages to modify */
	const uint16_t start_page = (y / PHEIGHT) > PAGES ? PAGES : y / PHEIGHT;
	uint16_t end_page = (y + ih + PHEIGHT - 1) / PHEIGHT;

	/* Shifts (in pixels) */
	const uint8_t shift = y % PHEIGHT;
	const uint8_t bottom_shift = (((y + ih) % PHEIGHT) != 0)
	        ? (uint8_t)(PHEIGHT - ((y + ih) % PHEIGHT)) : 0u;

	/* Pixel masks */
	const uint8_t mask = (uint8_t)(0xFFu << shift);
	const uint8_t bottom_mask = (uint8_t)(0xFFu >> bottom_shift);
	const uint8_t img_mask = (uint8_t)(mask & bottom_mask);

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
			if (i == start_page && img_idx < image->width) {
				top_byte = (canvas->data[j] & NOT(mask));
			} else {
				get_img_byte(image, img_idx - image->width,
				    &raw_byte, &alpha_mask);

				top_byte  = canvas->data[j]
				    & NOT(mask)
				    & NOT((alpha_mask >> (PHEIGHT - shift))
					& NOT(mask));
				top_byte |= (uint8_t)((raw_byte & alpha_mask)
				    >> (PHEIGHT - shift));
			}
			/* Byte with the current image data. */
			if (i == (end_page - 1)) {
				/* Image data at the end of the image. */
				get_img_byte(image, img_idx, &raw_byte,
				    &alpha_mask);

				img_byte = canvas->data[j]
				    & mask & NOT((alpha_mask << shift) & mask);
				img_byte = (uint8_t)(img_byte | ((img_mask != 0)
				    ? ((raw_byte & alpha_mask) << shift) : 0));
			} else {
				/* Image data somewhere before end. */
				get_img_byte(image, img_idx, &raw_byte,
				    &alpha_mask);

				img_byte = canvas->data[j]
				    & mask & NOT((alpha_mask << shift) & mask);
				img_byte |= (uint8_t)((raw_byte & alpha_mask)
				    << shift);
			}
			/* Data byte at the "bottom" of the current one. */
			if (i == (end_page - 1)) {
				bottom_byte = (uint8_t)
				    (canvas->data[j] & NOT(bottom_mask));
			} else {
				bottom_byte = 0;
			}

			/* Update byte on the canvas. */
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
xg_transfer_canvas(MSIM_SH1106_t *display, const xg_canvas_t *canvas)
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
xg_draw_scene(xg_canvas_t *canvas, const xg_scene_t *scene)
{
	const xg_layer_t *layer;
	const xg_anim_frame_t *frames;
	const xg_anim_frame_t *frame;
	xg_anim_t *anim;
	uint16_t rnd;
	uint16_t cache_idx;

	for (uint16_t i = scene->layers_n; i >= 1; i--) {
		layer = &scene->layers[i - 1];
		cache_idx = scene->layers_n - i; /* [0, layers_n - 1] */

		switch (layer->obj_type) {
		case XG_OT_IMG:
			if (cache_canvas != NULL &&
			    cached_layers >= (cache_idx + 1) &&
			    cache_pts[cache_idx].x == layer->base_pt.x &&
			    cache_pts[cache_idx].y == layer->base_pt.y) {
				/* Skip cached layer */
				continue;
			} else {
				xg_draw_pf(canvas, (const xg_image_t *)
				    layer->obj, layer->base_pt);

				/* Invalidate previously cached canvas */
				if (cache_state == CACHE_VALID) {
					cache_state = CACHE_INVALID;
					cached_layers = 0;
				}

				/* Cache this layer */
				if (cache_canvas != NULL) {
					cached_layers++;
					cache_pts[cache_idx] = layer->base_pt;
				}
			}
			break;
		case XG_OT_ANIM:
			anim = (xg_anim_t *) layer->obj;
			frames = (const xg_anim_frame_t *) anim->frames;
			frame = &frames[anim->frame_idx];

			/* Update cache state */
			if (cache_canvas != NULL &&
			    cached_layers == cache_idx &&
			    cache_state == CACHE_INVALID) {
				copy_canvas(cache_canvas, canvas);
				cache_state = CACHE_VALID;
			}

			/* Restore canvas from cache */
			if (cache_canvas != NULL &&
			    cached_layers == cache_idx &&
			    cache_state == CACHE_VALID) {
				copy_canvas(canvas, cache_canvas);
			}

			/* Don't draw an inactive animation. */
			if (anim->active == 0) {
				break;
			}

			/* Draw the current frame. */
			xg_draw_pf(canvas, frame->img, frame->base_pt);

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

/*
 * Provides an additional canvas to cache parts of a scene which might not
 * be changed, i.e. static images.
 */
int
xg_cache_canvas(xg_canvas_t *canvas)
{
	cache_canvas = canvas;
	cached_layers = 0;
	return 0;
}

static void
get_img_byte(const xg_image_t * const image, const uint32_t idx,
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

static void
copy_canvas(xg_canvas_t *dest, const xg_canvas_t *src)
{
	dest->width = src->width;
	dest->height = src->height;
	dest->data_size = src->data_size;
	memcpy(dest->data, src->data,
	    (src->width * src->height) / src->data_size);
}
