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
#ifndef XLING_GRAPHICS_H_
#define XLING_GRAPHICS_H_ 1

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106.h"

#include "xling/msg.h"

/* A callback function to handle keyboard input, interactive objects, etc. */
typedef void (*xg_cbk_t)(void *scene_ctx);

/*
 * Enumerator which denotes a type of a layer in the Xling scene. It helps to
 * draw different layers (images, animations, etc.) of a scene correctly.
 */
typedef enum xg_object_t {
	XG_OT_IMG,
	XG_OT_ANIM,
} xg_object_t;

typedef struct xg_point_t {
	int16_t			 x;
	int16_t			 y;
} xg_point_t;

typedef struct xg_layer_t {
	xg_point_t		 base_pt;
	void			*obj;
	xg_object_t		 obj_type;
} xg_layer_t;

/*
 * Scene represents a full state of the display at the given moment
 * in time. It might be composed of several images, animations,
 * interactive elements, text, and so on.
 *
 * A single image in GIMP represents a whole scene in Xling and
 * will be exported by the Xlingtool plugin accordingly.
 *
 * Please, note that a single layer in GIMP isn't always a single
 * layer in Xling scene. Several GIMP layers can form a single
 * layer with an animation, for example.
 *
 * layers
 *
 *     Pointer to an array of Xling objects (images, animations, etc.)
 *     which compose the scene. Each layer represents a single image
 *     (like layers organized in GIMP). The first one is the top layer,
 *     the last one is the bottom one.
 *
 * layers_n
 *
 *     Number of layers.
 */
typedef struct xg_scene_t {
	xg_layer_t		*layers;
	const uint16_t		 layers_n;
	xg_cbk_t		 kbd_cbk;
} xg_scene_t;

typedef struct xg_canvas_t {
	uint8_t			*data;
	uint16_t		 width;
	uint16_t		 height;
	uint16_t		 data_size;
} xg_canvas_t;

typedef struct xg_image_t {
	const uint8_t		*data;
	const uint8_t		*alpha;
	uint16_t		 width;
	uint16_t		 height;
	uint16_t		 data_size;
} xg_image_t;

/*
 * A single animation frame.
 *
 * It usually contains:
 *
 *  1. An image with visual data for this frame;
 *  2. A point to start painting this frame from (in relative coordinates
 *     which start from the layer's base point);
 *  3. Number of frame update cycles to stay at the screen;
 *  4. Frames counter to understand when to switch to the next animation frame.
 */
typedef struct xg_anim_frame_t {
	xg_point_t		 base_pt;
	const xg_image_t	*img;
	uint16_t		 alt;
	uint16_t		 alt_chance;
	uint16_t		 stay;
} xg_anim_frame_t;

typedef struct xg_anim_t {
	xg_anim_frame_t		*frames;
	const uint16_t		 frames_n;
	uint16_t		 frame_idx;
	uint16_t		 stay_cnt;
	uint8_t			 active;
} xg_anim_t;

typedef struct xg_glyph_t {
	uint32_t		 code;
	const uint8_t		*data;
	uint16_t		 width;
	uint16_t		 height;
	uint16_t		 data_size;
} xg_glyph_t;

typedef struct xg_font_t {
	uint32_t		 length;
	const xg_glyph_t	*glyphs;
} xg_font_t;

typedef struct xg_text_t {
	const xg_font_t		*font;
	char			*text;
	size_t			 text_sz;
} xg_text_t;

/* Scene context. */
typedef struct xg_scene_ctx_t {
	xg_scene_t		*scene;
	xg_canvas_t		*canvas;
	xg_text_t		*text;
	uint16_t		 frame_delay;
	uint16_t		 bat_lvl;
	uint16_t		 bat_stat;
	xm_btn_state_t		 btn_stat;
} xg_scene_ctx_t;

/* Xling graphics API */
int	xg_print(xg_canvas_t *canvas, const xg_text_t *text, xg_point_t p);
int	xg_draw_pf(xg_canvas_t *canvas, const xg_image_t *image, xg_point_t p);
int	xg_draw_scene(xg_canvas_t *canvas, const xg_scene_t *scene);
int	xg_cache_canvas(xg_canvas_t *canvas);
void	xg_transfer_canvas(MSIM_SH1106_t *display, const xg_canvas_t *canvas);

#endif /* XLING_GRAPHICS_H_ */
