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

#include <slope/drawing.h>

void slope_set_color (cairo_t *cr, SlopeRGBA color) {
    cairo_set_source_rgba(cr,
              slope_rgba_Rf(color),
              slope_rgba_Gf(color),
              slope_rgba_Bf(color),
              slope_rgba_Af(color));

}


void slope_move_to (cairo_t *cr, const SlopePoint *p) {
    cairo_move_to(cr, p->x, p->y);
}


void slope_line_to (cairo_t *cr, const SlopePoint *p) {
    cairo_line_to(cr, p->x, p->y);
}


void slope_rect (cairo_t *cr, const SlopeRect *r) {
    cairo_rectangle(cr, r->x, r->y, r->width, r->height);
}


void slope_draw (cairo_t *cr, SlopeRGBA stroke, SlopeRGBA fill) {
    if (slope_rgba_visible(stroke) && slope_rgba_visible(fill)) {
        slope_set_color(cr, fill);
        cairo_fill_preserve(cr);
        slope_set_color(cr, stroke);
        cairo_stroke(cr);
    } else if (slope_rgba_visible(stroke)) {
        slope_set_color(cr, stroke);
        cairo_stroke(cr);
    } else if (slope_rgba_visible(fill)) {
        slope_set_color(cr,fill);
        cairo_fill(cr);
    }
}
