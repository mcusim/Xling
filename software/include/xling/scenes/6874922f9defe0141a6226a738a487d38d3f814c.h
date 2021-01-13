/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of a firmware for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2020 Dmitry Salychev
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
#ifndef XG_IMG_6874922f9defe0141a6226a738a487d38d3f814c_H_
#define XG_IMG_6874922f9defe0141a6226a738a487d38d3f814c_H_ 1

#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>

/*
 * This file with monochrome image with 1-bit alpha channel has been generated
 * for Xling, a tamagotchi-like toy by LCD Image Converter.
 *
 * Data type: image
 * Filename: 6874922f9defe0141a6226a738a487d38d3f814c
 *
 * Preset name: Xling_SH1106_display_preset
 * Data block size: 8 bit(s), uint8_t
 * RLE compression: no
 * Conversion type: Monochrome, Diffuse Dither 128
 * Bits per pixel: 1
 * Bands used: yes
 * Band width: 8
 * Main scan direction: left_to_right
 * Line scan direction: forward
 * Inverse colors: no
 *
 * Xling, a tamagotchi-like toy: <https://github.com/mcusim/Xling>
 * LCD Image Converter: <https://www.riuson.com/lcd-image-converter>
 */

/* Xling graphics header */
#include "xling/graphics.h"

/* Header file with alpha channel for the image. */
#include "xling/scenes/6874922f9defe0141a6226a738a487d38d3f814c_a.h"


static const uint8_t PROGMEM XG_IMG_DATA_6874922f9defe0141a6226a738a487d38d3f814c[30] = {
    0x00, 0x00, 0x00, 0x00, 0xe0, 0xfc, 0xff, 0x3f, 0x7f, 0x7f, 0x7f, 0xef, 0xe3, 0xc0, 0x00, 0x00, 0x10, 0x18, 0x18, 0x18, 0x1b, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x07, 0x00, 0x00
};
XG_Image_t XG_IMG_6874922f9defe0141a6226a738a487d38d3f814c = {
        .data = XG_IMG_DATA_6874922f9defe0141a6226a738a487d38d3f814c,
	.alpha = XG_IMGA_6874922f9defe0141a6226a738a487d38d3f814c,
        .width = 15,
        .height = 14,
        .data_size = 8,
};

#endif /* XG_IMG_6874922f9defe0141a6226a738a487d38d3f814c_H_ */
