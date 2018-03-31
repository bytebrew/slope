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

#ifndef slope_geometry_h
#define slope_geometry_h

#include "slope/global.h"

typedef struct slope_point2d {
  slope_float_t x;
  slope_float_t y;
} slope_point2d_t;

typedef struct slope_point3d {
  slope_float_t x;
  slope_float_t y;
  slope_float_t z;
} slope_point3d_t;

typedef struct slope_rect2d {
  slope_float_t x;
  slope_float_t y;
  slope_float_t width;
  slope_float_t height;
} slope_rect2d_t;

/** @brief Returns the distance between p1 and p2 */
double slope_point2d_distance (
  slope_point2d_t *p1,
  slope_point2d_t *p2
);

/** @brief Returns the distance between p1 and p2 */
double slope_point3d_distance (
  slope_point3d_t *p1,
  slope_point3d_t *p2
);

/** @brief Returns 1 if r1 intersects r2 */
int slope_rect2d_intersects (
  slope_rect2d_t *r1,
  slope_rect2d_t *r2
);

#endif /* slope_geometry_h */
