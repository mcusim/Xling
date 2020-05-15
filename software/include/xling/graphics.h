/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of a firmware for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2019 Dmitry Salychev
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

/*
 * Xling graphics.
 */

/* SH1106 driver headers. */
#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106_graphics.h"

extern const MSIM_SH1106Image_t XG_IMG_EXY_IN_BOTTLE_01;
extern const MSIM_SH1106Image_t XG_IMG_EXY_IN_BOTTLE_02;
extern const MSIM_SH1106Image_t XG_IMG_BUTTERFLY;

#endif /* XG_GRAPHICS_H_ */
