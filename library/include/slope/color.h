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

#ifndef _slope_color_h_
#define _slope_color_h_

#include "slope/global.h"

G_BEGIN_DECLS

/*! \brief 32bit integer representing a color */
typedef guint32 SlopeRGBA;
#define SLOPE_RGBA(Val) ((SlopeRGBA) (Val))

#define SLOPE_WHITE                0xFFFFFFFF
#define SLOPE_RED                  0xFF0000FF
#define SLOPE_GREEN                0x00FF00FF
#define SLOPE_BLUE                 0x0000FFFF
#define SLOPE_BLACK                0x000000FF
#define SLOPE_COLOR_NULL           0x00000000
#define SLOPE_YELLOW               0xFFFF00FF
#define SLOPE_CYAN                 0x00FFFFFF
#define SLOPE_BLUEVIOLET           0x8A2BE2FF
#define SLOPE_BROWN                0xA52A2AFF
#define SLOPE_CADETBLUE            0x5F9EA0FF
#define SLOPE_CHOCOLATE            0xD2691EFF
#define SLOPE_DARKGOLDENROD        0xB8860BFF
#define SLOPE_DARKMAGENTA          0x8B008BFF
#define SLOPE_HOTPINK              0xFF69B4FF
#define SLOPE_LIGHTSKYBLUE         0x87CEFAFF
#define SLOPE_LIGHTSTEELBLUE       0xB0C4DEFF
#define SLOPE_MIDNIGHTBLUE         0x191970FF
#define SLOPE_MAROON               0x800000FF
#define SLOPE_TEAL                 0x008080FF
#define SLOPE_PALETURQUOISE        0xAFEEEEFF

/*! \brief Returns the red component of the color */
#define slope_get_red(Val)    ((SLOPE_RGBA(Val) >> 24U) & 0xFF)

/*! \brief Returns the green component of the color */
#define slope_get_green(Val)  ((SLOPE_RGBA(Val) >> 16U) & 0xFF)

/*! \brief Returns the blue component of the color */
#define slope_get_blue(Val)   ((SLOPE_RGBA(Val) >> 8U) & 0xFF)

/*! \brief Returns the alpha component of the color */
#define slope_get_alpha(Val)  (SLOPE_RGBA(Val) & 0xFF)

/*! \brief Sets the red component of the color */
#define slope_set_red(Val,x)    ((SLOPE_RGBA(Val) & 0x00FFFFFF) | ((SLOPE_RGBA(x) & 0xFF) << 24U))

/*! \brief Sets the green component of the color */
#define slope_set_green(Val,x)  ((SLOPE_RGBA(Val) & 0xFF00FFFF) | ((SLOPE_RGBA(x) & 0xFF) << 16U))

/*! \brief Sets the blue component of the color */
#define slope_set_blue(Val,x)   ((SLOPE_RGBA(Val) & 0xFFFF00FF) | ((SLOPE_RGBA(x) & 0xFF) << 8U))

/*! \brief Sets the alpha component of the color */
#define slope_set_alpha(Val,x)  ((SLOPE_RGBA(Val) & 0xFFFFFF00) | (SLOPE_RGBA(x) & 0xFF))

/*! \brief Returns a shade of gray between 0 (black) and 255 (white) */
#define slope_gray(x) ((((x) & 0xFF) << 24U) | (((x) & 0xFF) << 16U) | (((x) & 0xFF) << 8U) | 0xFF)

/*! \brief Returns TRUE if Color is visible */
#define slope_rgba_is_visible(Color) (slope_get_alpha(Color) > 0U)

G_END_DECLS

#endif /* _slope_color_h_ */
