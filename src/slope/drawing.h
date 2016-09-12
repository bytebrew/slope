/*
 * Copyright (C) 2016  Elvis Teixeira
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

#ifndef SLOPE_DRAWING_H
#define SLOPE_DRAWING_H

#include <slope/global.h>
#include <glib-object.h>
#include <cairo/cairo.h>

SLOPE_BEGIN_DECLS

typedef guint32              SlopeColor;
#define SLOPE_COLOR_NULL     0x00000000
#define SLOPE_BLACK          0x000000FF
#define SLOPE_WHITE          0xFFFFFFFF
#define SLOPE_RED            0xFF0000FF
#define SLOPE_GREEN          0x00FF00FF
#define SLOPE_BLUE           0x0000FFFF

#define SLOPE_GET_RED(color)       (((color)>>24)&0xFF)
#define SLOPE_GET_GREEN(color)     (((color)>>16)&0xFF)
#define SLOPE_GET_BLUE(color)      (((color)>>8)&0xFF)
#define SLOPE_GET_ALPHA(color)     ((color)&0xFF)
#define SLOPE_GET_REDF(color)      (((double)SLOPE_GET_RED(color))/255.0)
#define SLOPE_GET_GREENF(color)    (((double)SLOPE_GET_GREEN(color))/255.0)
#define SLOPE_GET_BLUEF(color)     (((double)SLOPE_GET_BLUE(color))/255.0)
#define SLOPE_GET_ALPHAF(color)    (((double)SLOPE_GET_ALPHA(color))/255.0)
#define SLOPE_COLOR_IS_NULL(color) (SLOPE_GET_ALPHA(color) == 0)


typedef struct
_SlopePoint
{
    double x;
    double y;
}
SlopePoint;

typedef struct
_SlopeRect
{
    double x;
    double y;
    double width;
    double height;
}
SlopeRect;



int slope_rect_contains (const SlopeRect *rect, double x, double y);

void slope_rect_translate (SlopeRect *rect, double dx, double dy);

void slope_cairo_set_color (cairo_t *cr, SlopeColor color);

void slope_cairo_line (cairo_t *cr, const SlopePoint *p1, const SlopePoint *p2);

void slope_cairo_rect (cairo_t *cr, const SlopeRect *r);

void slope_cairo_text (cairo_t *cr, double x, double y, const char *utf8);

void slope_cairo_circle (cairo_t *cr, const SlopePoint *center, double radius);

SLOPE_END_DECLS

#endif /* SLOPE_DRAWING_H */
