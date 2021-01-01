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
#ifndef XG_IMG_da5943699c956ba173da6469f07c3a392427d4bf_H_
#define XG_IMG_da5943699c956ba173da6469f07c3a392427d4bf_H_ 1

#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>

/*
 * This file with monochrome image with 1-bit alpha channel has been generated
 * for Xling, a tamagotchi-like toy by LCD Image Converter.
 *
 * Data type: image
 * Filename: da5943699c956ba173da6469f07c3a392427d4bf
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
#include "xling/scenes/da5943699c956ba173da6469f07c3a392427d4bf_a.h"


static const uint8_t PROGMEM XG_IMG_DATA_da5943699c956ba173da6469f07c3a392427d4bf[9] = {
    0x13, 0x01, 0x01, 0x00, 0x00, 0x00, 0x06, 0x07, 0x13
};
const XG_Image_t XG_IMG_da5943699c956ba173da6469f07c3a392427d4bf = {
        .data = XG_IMG_DATA_da5943699c956ba173da6469f07c3a392427d4bf,
	.alpha = XG_IMGA_da5943699c956ba173da6469f07c3a392427d4bf,
        .width = 9,
        .height = 5,
        .data_size = 8,
};

#endif /* XG_IMG_da5943699c956ba173da6469f07c3a392427d4bf_H_ */
