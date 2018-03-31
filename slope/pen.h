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

#ifndef slope_pen_h
#define slope_pen_h

#include "slope/color.h"
#include "slope/geometry.h"

/** @brief Opaque pen type */
typedef struct slope_pen slope_pen_t;
#define SLOPE_PEN(self) ((slope_pen_t*) (self))


/** @brief Destroys a pen*/
void slope_pen_destroy (
  slope_pen_t *self
);

/** @brief Starts a new pen path */
void slope_pen_begin_path (
  slope_pen_t *self
);

/** @brief Ends a pen path */
void slope_pen_end_path (
  slope_pen_t *self
);

/** @brief Moves the pen without drawing */
void slope_pen_move_to (
  slope_pen_t *self,
  slope_float_t x,
  slope_float_t y
);

/** @brief Moves the pen drawing to the output */
void slope_pen_line_to (
  slope_pen_t *self,
  slope_float_t x,
  slope_float_t y
);

/** @brief Closes the current path */
void slope_pen_close_path (
  slope_pen_t *self
);

/** @brief Sets the pen colors */
void slope_pen_set_colors (
  slope_pen_t *self,
  slope_color_t stroke,
  slope_color_t fill
);

#endif /* slope_pen_h */
