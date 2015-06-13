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
#include "slope-config.h"

#include <cairo.h>
#if SLOPE_HAVE_PANGO
# include <pango/pangocairo.h>
#endif


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


void slope_draw_text(cairo_t *cr,
                     double x, double y,
                     const char *text)
{
#if SLOPE_HAVE_PANGO
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, text, -1);
    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);
    g_object_unref(layout);
#else
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);
#endif /* SLOPE_HAVE_PANGO */
}


void slope_get_text_rect(cairo_t *cr,
                         slope_rect_t *rect, const char *text)
{
    #if SLOPE_HAVE_PANGO
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, text, -1);
    pango_cairo_update_layout(cr, layout);
    rect->width = pango_layout_get_width(layout);
    rect->height = pango_layout_get_height(layout);
    g_object_unref(layout);
    #else
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, text, &txt_ext);
    rect->width = txt_ext.width;
    rect->height = txt_ext.height;
    #endif /* SLOPE_HAVE_PANGO */
}

/* slope/primitives.c */
