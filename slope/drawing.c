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

#include <stdlib.h>
#include "slope/svg.h"

int main()
{
  slope_pen_t *pen =
      slope_svgpen_new("drawing.html", 300, 300, SLOPE_SVG_OUTPUT_HTML);

  /* triangle */
  slope_pen_set_colors(pen, SLOPE_BLUE, SLOPE_GREEN);
  slope_pen_set_width(pen, 2.0);
  slope_pen_begin_path(pen);
  slope_pen_move_to(pen, 0.0, 0.0);
  slope_pen_line_to(pen, 100.0, 100.0);
  slope_pen_line_to(pen, 100.0, 200.0);
  slope_pen_close_path(pen);
  slope_pen_end_path(pen);

  /* circle */
  slope_pen_set_colors(pen, SLOPE_BLUE, SLOPE_RED);
  slope_pen_set_width(pen, 4.0);
  slope_pen_circle(pen, 200, 200, 50);

  slope_pen_destroy(pen);
  return 0;
}
