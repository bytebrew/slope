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
typedef guint32 SlopeRGB;
#define SLOPE_RGB(Val) ((SlopeRGB) (Val))

#define SlopeRGB_Black            0x000000FF
#define SlopeRGB_White            0xFFFFFFFF
#define SlopeRGB_Red              0xFF0000FF
#define SlopeRGB_Green            0x00FF00FF
#define SlopeRGB_Blue             0x0000FFFF

/*! \brief Returns the red component of the color */
#define slope_rgb_get_red(Val)    ((SLOPE_RGB(Val) >> 24U) & 0xFF)

/*! \brief Returns the green component of the color */
#define slope_rgb_get_green(Val)  ((SLOPE_RGB(Val) >> 16U) & 0xFF)

/*! \brief Returns the blue component of the color */
#define slope_rgb_get_blue(Val)   ((SLOPE_RGB(Val) >> 8U) & 0xFF)

/*! \brief Returns the alpha component of the color */
#define slope_rgb_get_alpha(Val)  (SLOPE_RGB(Val) & 0xFF)

/*! \brief Sets the red component of the color */
#define slope_rgb_set_red(Val,x)    ((SLOPE_RGB(Val) & 0x00FFFFFF) | ((SLOPE_RGB(x) & 0xFF) << 24U))

/*! \brief Sets the green component of the color */
#define slope_rgb_set_green(Val,x)  ((SLOPE_RGB(Val) & 0xFF00FFFF) | ((SLOPE_RGB(x) & 0xFF) << 16U))

/*! \brief Sets the blue component of the color */
#define slope_rgb_set_blue(Val,x)   ((SLOPE_RGB(Val) & 0xFFFF00FF) | ((SLOPE_RGB(x) & 0xFF) << 8U))

/*! \brief Sets the alpha component of the color */
#define slope_rgb_set_alpha(Val,x)  ((SLOPE_RGB(Val) & 0xFFFFFF00) | (SLOPE_RGB(x) & 0xFF))

/*! \brief Returns a shade of gray between 0 (black) and 255 (white) */
#define slope_rgb_gray(x) ((((x) & 0xFF) << 24U) | (((x) & 0xFF) << 16U) | (((x) & 0xFF) << 8U) | 0xFF)

G_END_DECLS

#endif /* _slope_color_h_ */
