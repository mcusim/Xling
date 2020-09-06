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
#ifndef XG_GRAPHICS_H_
#define XG_GRAPHICS_H_ 1

/* SH1106 driver headers. */
#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106.h"

/*
 * -----------------------------------------------------------------------------
 * Xling graphics types (images, scenes, text, etc.)
 * -----------------------------------------------------------------------------
 */
typedef struct XG_Scene_t  XG_Scene_t;
typedef struct XG_Layer_t  XG_Layer_t;
typedef struct XG_Canvas_t XG_Canvas_t;
typedef struct XG_Image_t  XG_Image_t;
typedef struct XG_Point_t  XG_Point_t;

typedef struct XG_Text_t   XG_Text_t;
typedef struct XG_Glyph_t  XG_Glyph_t;
typedef struct XG_Font_t   XG_Font_t;

typedef enum XG_ObjectType_t {
	XG_OT_Image,
	XG_OT_Animation,
} XG_ObjectType_t;

struct XG_Point_t {
	uint16_t          x;
	uint16_t          y;
};

/*
 * Scene represents a full state of the display at the given moment
 * in time. It might be composed of several images, animations,
 * interactive elements, text, and so on.
 *
 * A single image in GIMP represents a whole scene in Xling and
 * will be exported by the xlingtool plugin accordingly.
 *
 * Please, note that a single layer in GIMP isn't always a single
 * layer in Xling. Several GIMP layers can form a single layer with
 * some animation, for example.
 *
 * layers
 *
 *     Pointer to an array of Xling images which compose the scene.
 *     Each layer represents a single image (like layers organized
 *     in GIMP). The first one is the top layer, the last one is
 *     the bottom one.
 *
 * layers_n
 *
 *     Number of layers.
 */
struct XG_Scene_t {
	const XG_Layer_t *layers;
	const uint16_t    layers_n;
};

struct XG_Layer_t {
	const void       *obj;
	XG_Point_t        base_pt;
	XG_ObjectType_t   obj_type;
};

struct XG_Canvas_t {
	uint8_t          *data;
	uint16_t          width;
	uint16_t          height;
	uint16_t          data_size;
};

struct XG_Image_t {
	const uint8_t    *data;
	const uint8_t    *alpha;
	uint16_t          width;
	uint16_t          height;
	uint16_t          data_size;
};

struct XG_Glyph_t {
	uint32_t          code;
	const uint8_t    *data;
	uint16_t          width;
	uint16_t          height;
	uint16_t          data_size;
};

struct XG_Font_t {
	uint32_t          length;
	const XG_Glyph_t *glyphs;
};

struct XG_Text_t {
	const XG_Font_t  *font;
	const char       *text;
};

/*
 * -----------------------------------------------------------------------------
 * Xling graphics API.
 * -----------------------------------------------------------------------------
 */
int    XG_Print(XG_Canvas_t *canvas, const XG_Text_t *text, XG_Point_t p);
int    XG_Draw_PF(XG_Canvas_t *canvas, const XG_Image_t *image, XG_Point_t p);
int    XG_DrawScene(XG_Canvas_t *canvas, const XG_Scene_t *scene);
void   XG_TransferCanvas(MSIM_SH1106_t *display, const XG_Canvas_t *canvas);

#endif /* XG_GRAPHICS_H_ */
