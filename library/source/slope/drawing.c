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

#include "slope/drawing.h"

void slope_cairo_set_rgba (cairo_t *cr, SlopeRGB color)
{
    cairo_set_source_rgba(cr,
        (double) slope_rgb_get_red(color),
        (double) slope_rgb_get_green(color),
        (double) slope_rgb_get_blue(color),
        (double) slope_rgb_get_alpha(color));
}

void slope_draw_rect (cairo_t *cr, const SlopeRect *rect)
{
    cairo_rectangle(cr, rect->x, rect->y, rect->width, rect->height);
}

/* slope/drawing.c */
