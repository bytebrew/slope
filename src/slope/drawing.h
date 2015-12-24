/*
 * Copyright (C) 2015  Elvis Teixeira
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

#ifndef __SLOPE_DRAWING__
#define __SLOPE_DRAWING__

#include <slope/global.h>
#include <cairo/cairo.h>

/* shortcut for creating a color from bgba values */
#define SLOPE_RGBA(r,g,b,a) \
    ((slope_color_t) (((r)&0xff)<<24 \
                   | ((g)&0xff)<<16 \
                   | ((b)&0xff)<<8 \
                   | (a)))

/* named color values */
#define SLOPE_BLACK       0x000000FF
#define SLOPE_WHITE       0xFFFFFFFF
#define SLOPE_GRAY        0x808080FF
#define SLOPE_LIGHTGRAY   0x66666666
#define SLOPE_DARKGRAY    0xAAAAAAFF
#define SLOPE_SLATEGRAY   0x778899FF
#define SLOPE_MAROON      0x800000FF
#define SLOPE_RED         0xFF0000FF
#define SLOPE_GREEN       0x00FF00FF
#define SLOPE_BLUE        0x0000FFFF
#define SLOPE_YELLOW      0xFFFF00FF
#define SLOPE_MAGENTA     0xFF00FFFF
#define SLOPE_CYAN        0x00FFFFFF
#define SLOPE_AZURE       0xF0FFFFFF
#define SLOPE_VIOLET      0xEE82FFFF
#define SLOPE_ORANGE      0xFF8C00FF
#define SLOPE_INDIGO      0x4B0082FF
#define SLOPE_COLOR_NULL  0x00000000

/* get rgb components from color */
#define SLOPE_COLOR_GET_RED(color)    (((color) >> 24) & 0xff)
#define SLOPE_COLOR_GET_GREEN(color)  (((color) >> 16) & 0xff)
#define SLOPE_COLOR_GET_BLUE(color)   (((color) >> 8) & 0xff)
#define SLOPE_COLOR_GET_ALPHA(color)  ((color) & 0xff)

/* a null color name, totally transparent */
#define SLOPE_COLOR_IS_NULL(color) ((color) == SLOPE_COLOR_NULL)

SLOPE_BEGIN_DECLS

typedef struct slope_point {
   double x;
   double y;
} slope_point_t;

typedef struct slope_rect {
   double x;
   double y;
   double width;
   double height;
} slope_rect_t;


typedef unsigned int slope_color_t;


void slope_rect_set (slope_rect_t *self, double x, double y, double width, double height);
void slope_rect_copy (slope_rect_t *dest, const slope_rect_t *source);
void slope_cairo_rect (cairo_t *cr, const slope_rect_t *rect);
void slope_cairo_line (cairo_t *cr, const slope_point_t *p1, const slope_point_t *p2);
void slope_cairo_circle (cairo_t *cr, const slope_point_t *center, double radius);
void slope_cairo_set_color (cairo_t *cr, const slope_color_t color);

SLOPE_END_DECLS

#endif /*__SLOPE_DRAWING__*/
