/*
 * Copyright (C) 2018 Elvis M. Teixeira
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <slope/theme.h>
#include <cairo/cairo.h>

int main(int argc, char *argv[])
{
  cairo_surface_t *surf = cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, 500, 500);
  cairo_t *cr = cairo_create(surf);

  SlopeColor color = slope_default_item_themes[0].colors[0];

  cairo_set_source_rgba(cr,
        slope_get_red(color),
        slope_get_green(color),
        slope_get_blue(color),
        slope_get_alpha(color));

  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, 100, 100);
  cairo_stroke(cr);

  cairo_surface_write_to_png(
        surf, "figure.png");
  return 0;
}
