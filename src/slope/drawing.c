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

#include <slope/drawing.h>

void slope_rect_set (slope_rect_t *self, double x, double y, double width, double height)
{
    self->x = x;
    self->y = y;
    self->width = width;
    self->height = height;
}


void slope_rect_copy (slope_rect_t *dest, const slope_rect_t *source)
{
    dest->x = source->x;
    dest->y = source->y;
    dest->width = source->width;
    dest->height = source->height;
}


void slope_cairo_rect (cairo_t *cr, const slope_rect_t *rect)
{
    cairo_rectangle(cr, rect->x, rect->y, rect->width, rect->height);
}


void slope_cairo_line (cairo_t *cr, const slope_point_t *p1, const slope_point_t *p2)
{
    cairo_move_to(cr, p1->x, p1->y);
    cairo_line_to(cr, p2->x, p2->y);
}


void slope_cairo_circle (cairo_t *cr, const slope_point_t *center, double radius)
{
    cairo_move_to(cr, center->x + radius, center->y);
    cairo_arc(cr, center->x, center->y, radius, 0.0, 6.283185307179587);
}


void slope_cairo_set_color (cairo_t *cr, const slope_color_t color)
{
    int a = SLOPE_COLOR_GET_ALPHA(color);
    int b = SLOPE_COLOR_GET_BLUE(color);
    int g = SLOPE_COLOR_GET_GREEN(color);
    int r = SLOPE_COLOR_GET_RED(color);
    
    cairo_set_source_rgba(cr,
        ((double) r) / 255.0,
        ((double) g) / 255.0,
        ((double) b) / 255.0,
        ((double) a) / 255.0
    );
}

/* slope/drawing.h */