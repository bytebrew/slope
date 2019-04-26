/*
 * Copyright (C) 2019  Elvis Teixeira
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

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <slope/color.h>
#include <cairo/cairo.h>

G_BEGIN_DECLS

typedef struct {
    double x;
    double y;
} SlopePoint;

typedef struct {
    double x;
    double y;
    double width;
    double height;
} SlopeRect;


void slope_set_color (cairo_t *cr, SlopeRGBA color);

void slope_move_to (cairo_t *cr, const SlopePoint *p);

void slope_line_to (cairo_t *cr, const SlopePoint *p);

void slope_rect (cairo_t *cr, const SlopeRect *r);

void slope_draw (cairo_t *cr, SlopeRGBA stroke, SlopeRGBA fill);

G_END_DECLS

#endif /* SLOPE_DRAWING_H */
