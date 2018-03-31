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

#ifndef slope_pen_p_h
#define slope_pen_p_h

#include "slope/pen.h"
#include <stdlib.h>
#include <string.h>

#ifndef NULL
# define NULL ((void*) 0)
#endif

#define slope_new(type) ((type*) malloc(sizeof(type)))
#define slope_free(ptr) free(ptr)


typedef struct slope_pen_class {
  /*
   * class-wide traits
   */
  slope_uint32_t traits;

  /*
   * virtual functions
   */
  void (*destroy) (
    slope_pen_t *self
  );
  void (*begin_path) (
    slope_pen_t *self
  );
  void (*end_path) (
    slope_pen_t *self
  );
  void (*move_to) (
    slope_pen_t *self,
    slope_float_t x,
    slope_float_t y
  );
  void (*line_to) (
    slope_pen_t *self,
    slope_float_t x,
    slope_float_t y
  );
  void (*set_colors) (
    slope_pen_t *self,
    slope_color_t stroke,
    slope_color_t fill
  );
  void (*close_path) (
    slope_pen_t *self
  );
  void (*circle) (
    slope_pen_t *self,
    slope_float_t x,
    slope_float_t y,
    slope_float_t r
  );
  void (*set_width) (
    slope_pen_t *self,
    slope_float_t width
  );
  void (*text) (
    slope_pen_t *self,
    slope_float_t x,
    slope_float_t y,
    const char *text
  );
} slope_pen_class_t;


struct slope_pen {
  /*
   * instance-wide traits
   */
  slope_uint32_t traits;
  /*
   * class pointer
   */
  slope_pen_class_t *klass;
};

#endif /* slope_pen_p_h */
