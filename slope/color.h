/*
 * Copyright (C) 2018  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef slope_color_h
#define slope_color_h

#include "slope/global.h"

typedef slope_uint32_t slope_color_t;

#define slope_color_get_red(color) \
  (((color) >> 24) & 0xff)

#define slope_color_get_green(color) \
  (((color) >> 16) & 0xff)

#define slope_color_get_blue(color) \
  (((color) >> 8) & 0xff)

#define slope_color_get_alpha(color) \
  ((color) & 0xff)

#define slope_color_set_red(color, value) \
    (((color) & 0x00ffffff) | (((value) & 0xff) << 24))

#define slope_color_set_green(color, value) \
  (((color) & 0xff00ffff) | (((value) & 0xff) << 16))

#define slope_color_set_blue(color, value) \
  (((color) & 0xffff00ff) | (((value) & 0xff) << 8))

#define slope_color_set_alpha(color, value) \
  (((color) & 0xffffff00) | ((value) & 0xff))

#define slope_color_is_visible(color) \
  (slope_color_get_alpha(color) != 0)

#define slope_color_gray(value) \
  ((((value) & 0xff) << 24) | \
  (((value) & 0xff) << 16) | \
  (((value) & 0xff) << 8) | \
  ((value) & 0xff))


#define SLOPE_NO_COLOR      0x00000000
#define SLOPE_RED           0xff0000ff
#define SLOPE_GREEN         0x00ff00ff
#define SLOPE_BLUE          0x0000ffff
#define SLOPE_WHITE         0xffffffff
#define SLOPE_BLACK         0x000000ff

#endif /* slope_color_h */
