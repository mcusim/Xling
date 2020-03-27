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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*
 * Driver should be aware of the display connected via one of the interfaces
 * mentioned below.
 *
 * NOTE: Interface-specific implementations can be found in the "sh1106_*.c"
 * files, like "sh1106_spi4.c" for 4-wire SPI interface.
 */

#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106.h"

#if !defined(configMSIM_DRV_DISPLAY_SH1106_TWIBB) && \
    !defined(configMSIM_DRV_DISPLAY_SH1106_TWI) && \
    !defined(configMSIM_DRV_DISPLAY_SH1106_SPI4)
#error "Please, let the driver know how the SH1106-based display is connected."
#endif
