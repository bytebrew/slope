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

#define SLOPE_RGBA(r,g,b,a) ((slope_color_t) (((r)&0xff)<<24 | ((g)&0xff)<<16 | ((b)&0xff)<<8 | (a)))

#define SLOPE_BLACK       SLOPE_RGBA(0,0,0,255)
#define SLOPE_WHITE       SLOPE_RGBA(255,255,255,255)
#define SLOPE_GRAY        SLOPE_RGBA(127,127,127,255)
#define SLOPE_LIGHTGRAY   SLOPE_RGBA(80,80,80,80)
#define SLOPE_DARKGRAY    SLOPE_RGBA(190,190,190,255)
#define SLOPE_RED         SLOPE_RGBA(255,0,0,255)
#define SLOPE_GREEN       SLOPE_RGBA(0,255,0,255)
#define SLOPE_BLUE        SLOPE_RGBA(0,0,255,255)
#define SLOPE_COLOR_NULL  SLOPE_RGBA(0,0,0,0)

#define SLOPE_COLOR_GET_RED(color)    (((color) >> 24) & 0xff)
#define SLOPE_COLOR_GET_GREEN(color)  (((color) >> 16) & 0xff)
#define SLOPE_COLOR_GET_BLUE(color)   (((color) >> 8) & 0xff)
#define SLOPE_COLOR_GET_ALPHA(color)  ((color) & 0xff)

#define SLOPE_COLOR_IS_NULL(color) (SLOPE_COLOR_GET_ALPHA(color) == 0)

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
void slope_cairo_set_color (cairo_t *cr, const slope_color_t color);

SLOPE_END_DECLS

#endif /*__SLOPE_DRAWING__*/
