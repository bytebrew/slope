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

#include <slope/drawing.h>


int slope_rect_contains (const SlopeRect *rect, double x, double y)
{
    if (x < rect->x || (rect->x + rect->width) <= x) return 0;
    if (y <= rect->y || (rect->y + rect->height) < y) return 0;
    return 1;
}


void slope_rect_translate (SlopeRect *rect, double dx, double dy)
{
    rect->x += dx;
    rect->y += dy;
}


void slope_cairo_set_color (cairo_t *cr, SlopeColor color)
{
    cairo_set_source_rgba(cr,
        SLOPE_GET_REDF(color),
        SLOPE_GET_GREENF(color),
        SLOPE_GET_BLUEF(color),
        SLOPE_GET_ALPHAF(color));
}


void slope_cairo_line (cairo_t *cr, const SlopePoint *p1, const SlopePoint *p2)
{
    cairo_move_to(cr, p1->x, p1->y);
    cairo_line_to(cr, p2->x, p2->y);
}


void slope_cairo_rect (cairo_t *cr, const SlopeRect *r)
{
    cairo_rectangle(cr, r->x, r->y, r->width, r->height);
}


void slope_cairo_text (cairo_t *cr, double x, double y, const char *utf8)
{
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, utf8);
}


void slope_cairo_circle (cairo_t *cr, const SlopePoint *center, double radius)
{
    cairo_move_to(cr, center->x + radius, center->y);
    cairo_arc(cr, center->x, center->y, radius, 0.0, 6.28318530717959);
}

/* slope/drawing.c */
