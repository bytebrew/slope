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
            slope_color_set(color, 0.0, 0.0, 0.0, 1.0);
            break;
        case SLOPE_WHITE:
            slope_color_set(color, 1.0, 1.0, 1.0, 1.0);
            break;
        case SLOPE_RED:
            slope_color_set(color, 1.0, 0.0, 0.0, 1.0);
            break;
        case SLOPE_GREEN:
            slope_color_set(color, 0.0, 1.0, 0.0, 1.0);
            break;
        case SLOPE_BLUE:
            slope_color_set(color, 0.0, 0.0, 1.0, 1.0);
            break;
        case SLOPE_YELLOW:
            slope_color_set(color, 1.0, 1.0, 0.0, 1.0);
            break;
        case SLOPE_MAROON:
            slope_color_set(color, 0.5, 0.0, 0.0, 1.0);
            break;
        case SLOPE_GREY:
            slope_color_set(color, 0.5, 0.5, 0.5, 1.0);
            break;
        default: /* SLOPE_BLACK */
            slope_color_set(color, 0.0, 0.0, 0.0, 1.0);
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


void slope_draw_text(cairo_t *cr, slope_font_t *font,
                     double x, double y, const char *text)
{
#if SLOPE_HAVE_PANGO
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description (layout, font->font);
    pango_layout_set_text(layout, text, -1);
    pango_cairo_update_layout(cr, layout);
    cairo_move_to(cr, x, y);
    pango_cairo_show_layout_line(cr, pango_layout_get_line (layout, 0));
    g_object_unref(layout);
#else
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);
#endif /* SLOPE_HAVE_PANGO */
}


void slope_get_text_rect(cairo_t *cr, slope_font_t *font,
                         slope_rect_t *rect, const char *text)
{
    #if SLOPE_HAVE_PANGO
    int width, height;
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description (layout, font->font);
    pango_layout_set_text(layout, text, -1);
    pango_cairo_update_layout(cr, layout);
    pango_layout_get_pixel_size(layout, &width, &height);
    rect->width = (double) width;
    rect->height = (double) height;
    g_object_unref(layout);
    #else
    cairo_text_extents_t txt_ext;
    cairo_text_extents(cr, text, &txt_ext);
    rect->width = txt_ext.width;
    rect->height = txt_ext.height;
    #endif /* SLOPE_HAVE_PANGO */
}

/* slope/primitives.c */
