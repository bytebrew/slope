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

#ifndef _slope_drawing_h_
#define _slope_drawing_h_

#include "slope/color.h"
#include <cairo.h>

G_BEGIN_DECLS

typedef struct
_SlopePoint {
    double x;
    double y;
} SlopePoint;


typedef struct
_SlopeRect {
    double x;
    double y;
    double width;
    double height;
} SlopeRect;


void slope_cairo_set_rgba (cairo_t *cr, SlopeRGB color);

void slope_draw_rect (cairo_t *cr, const SlopeRect *rect);

void slope_draw_round_rect (cairo_t *cr, const SlopeRect *rec, double radius);

void slope_draw (cairo_t *cr, double stroke_width, SlopeRGB fill_color, SlopeRGB stroke_color);

G_END_DECLS

#endif /* _slope_drawing_h_ */
