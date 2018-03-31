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

#ifndef slope_eps_pen_p_h
#define slope_eps_pen_p_h

#include "slope/pen_p.h"
#include "slope/eps.h"
#include <stdio.h>

#define SLOPE_EPS_OP_IN_PROGRESS  (1U << 15U)
#define SLOPE_EPS_INSIDE_PATH     ((1U << 16U) | SLOPE_EPS_OP_IN_PROGRESS)


extern slope_pen_class_t slope_epspen_class;

struct slope_epspen {
  slope_pen_t base;
  slope_uint32_t opts;
  char *file_name;
  FILE *file;
  slope_point2d_t pos;
  slope_color_t fill_color;
  slope_color_t stroke_color;
  slope_float_t stroke_width;
};

void slope_epspen_destroy (
  slope_pen_t *self
);

void slope_epspen_initialize_file (
  slope_epspen_t *self,
  slope_float_t width,
  slope_float_t height
);

void slope_epspen_finalize_file (
  slope_epspen_t *self
);

void slope_epspen_begin_path (
  slope_pen_t *self
);

void slope_epspen_end_path (
  slope_pen_t *self
);

void slope_epspen_move_to (
  slope_pen_t *self,
  slope_float_t x,
  slope_float_t y
);

void slope_epspen_line_to (
  slope_pen_t *self,
  slope_float_t x,
  slope_float_t y
);

void slope_epspen_close_path (
  slope_pen_t *self
);

void slope_epspen_set_colors (
  slope_pen_t *self,
  slope_color_t stroke,
  slope_color_t fill
);

void slope_epspen_circle (
  slope_pen_t *self,
  slope_float_t x,
  slope_float_t y,
  slope_float_t r
);

void slope_epspen_set_width (
  slope_pen_t *self,
  slope_float_t width
);

void slope_epspen_text (
  slope_pen_t *self,
  slope_float_t x,
  slope_float_t y,
  const char *text
);

#endif /* slope_epspen_p_h */
