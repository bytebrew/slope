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

#ifndef slope_svg_h
#define slope_svg_h

#include "slope/pen.h"

#define SLOPE_SVG_NONE_OPT       (0U)
#define SLOPE_SVG_OUTPUT_HTML    (1U)

typedef struct slope_svgpen slope_svgpen_t;
#define SLOPE_SVGPEN(self) ((slope_svgpen_t*) (self))

/** @brief Creates a new SVG pen */
slope_pen_t* slope_svgpen_new(
  const char *file_name,
  slope_float_t width,
  slope_float_t height,
  slope_uint32_t opts
);

/** @brief Toggle HTML output */
void slope_svgpen_set_html_output(
  slope_svgpen_t *self,
  slope_bool_t enable
);

#endif /* slope_svg_h */
