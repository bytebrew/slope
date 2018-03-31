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
#define SLOPE_WHITE 0xFFFFFFFF
#define SLOPE_RED 0xFF0000FF
#define SLOPE_GREEN 0x00FF00FF
#define SLOPE_BLUE 0x0000FFFF
#define SLOPE_BLACK 0x000000FF
#define SLOPE_YELLOW 0xFFFF00FF
#define SLOPE_CYAN 0x00FFFFFF
#define SLOPE_BLUEVIOLET 0x8A2BE2FF
#define SLOPE_BROWN 0xA52A2AFF
#define SLOPE_CADETBLUE 0x5F9EA0FF
#define SLOPE_CHOCOLATE 0xD2691EFF
#define SLOPE_DARKGOLDENROD 0xB8860BFF
#define SLOPE_DARKMAGENTA 0x8B008BFF
#define SLOPE_HOTPINK 0xFF69B4FF
#define SLOPE_LIGHTSKYBLUE 0x87CEFAFF
#define SLOPE_LIGHTSTEELBLUE 0xB0C4DEFF
#define SLOPE_MIDNIGHTBLUE 0x191970FF
#define SLOPE_MAROON 0x800000FF
#define SLOPE_TEAL 0x008080FF
#define SLOPE_PALETURQUOISE 0xAFEEEEFF

#endif /* slope_color_h */
