#!/bin/sh
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# This file is part of a firmware for Xling, a tamagotchi-like toy.
#
# Copyright (c) 2019, 2020 Dmitry Salychev
#
# Xling firmware is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Xling firmware is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

./lcd-image-converter \
	--mode=convert-image \
	--doc-name=TEST_ALPHA_02 \
	--preset-name=Xling_SH1106_display_preset_alpha_channel \
	--template=./Xling_image_alpha_template.tmpl \
	--input=./test_alpha_02.png \
	--output=./test_alpha_02.c

./lcd-image-converter --mode=convert-image --doc-name=smoking_02 --preset-name=Xling_SH1106_display_preset --template=./Xling_image_template.tmpl --input=./smoking_02.png --output=./smoking_02.h
