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


void slope_draw_round_rect (cairo_t *cr, const SlopeRect *rec, double radius)
{
  static const double degrees = 0.01745329252; /* ~pi/180 */

  cairo_new_sub_path(cr);
  cairo_arc(cr, rec->x + rec->width - radius,
            rec->y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc(cr, rec->x + rec->width - radius,
            rec->y + rec->height - radius, radius,
            0 * degrees, 90 * degrees);
  cairo_arc(cr, rec->x + radius, rec->y + rec->height - radius,
            radius, 90 * degrees, 180 * degrees);
  cairo_arc(cr, rec->x + radius, rec->y + radius,
            radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);
}


void slope_draw (cairo_t *cr, double stroke_width,
                 SlopeRGB fill_color, SlopeRGB stroke_color)
{
    if (slope_rgb_is_visible(stroke_color)) {
        if (slope_rgb_is_visible(fill_color)) {
            slope_cairo_set_rgba (cr, fill_color);
            cairo_fill_preserve (cr);
        }
        cairo_set_line_width (cr, stroke_width);
        slope_cairo_set_rgba (cr, stroke_color);
        cairo_stroke (cr);
    } else if (slope_rgb_is_visible(fill_color)) {
        slope_cairo_set_rgba (cr, fill_color);
        cairo_fill (cr);
    }
}

/* slope/drawing.c */
