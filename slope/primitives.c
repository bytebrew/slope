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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "slope/primitives.h"
#include <cairo.h>


void slope_rect_set (slope_rect_t *rect, double x,
                     double y, double w, double h)
{
    rect->x = x;
    rect->y = y;
    rect->width = w;
    rect->height = h;
}


void slope_color_set (slope_color_t *color, double r,
                      double g, double b, double a)
{
    color->red = r;
    color->green = g;
    color->blue = b;
    color->alpha = a;
}


void slope_color_set_name (slope_color_t *color,
                           slope_color_name_t name)
{
    color->alpha = 1.0;
    switch (name) {
        case SLOPE_BLACK:
            color->red = 0.0;
            color->green = 0.0;
            color->blue = 0.0;
            break;
        case SLOPE_WHITE:
            color->red = 1.0;
            color->green = 1.0;
            color->blue = 1.0;
            break;
        case SLOPE_RED:
            color->red = 1.0;
            color->green = 0.0;
            color->blue = 0.0;
            break;
        case SLOPE_GREEN:
            color->red = 0.0;
            color->green = 1.0;
            color->blue = 0.0;
            break;
        case SLOPE_BLUE:
            color->red = 0.0;
            color->green = 0.0;
            color->blue = 1.0;
            break;
        case SLOPE_YELLOW:
            color->red = 1.0;
            color->green = 1.0;
            color->blue = 0.0;
            break;
        case SLOPE_MAROON:
            color->red = 0.5;
            color->green = 0.0;
            color->blue = 0.0;
            break;
        case SLOPE_GREY:
            color->red = 0.5;
            color->green = 0.5;
            color->blue = 0.5;
            break;
    }
}


void slope_cairo_set_color(cairo_t *cr,
                           const slope_color_t *color)
{
    cairo_set_source_rgba(
        cr, color->red, color->green, 
        color->blue, color->alpha);
}


void slope_cairo_rectangle(cairo_t *cr,
                           const slope_rect_t *rect)
{
    cairo_rectangle(cr, rect->x, rect->y,
                    rect->width, rect->height);
}

/* slope/primitives.c */

