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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_LINEAR_P__
#define __SLOPE_LINEAR_P__

#include <slope/linear.h>
#include <slope/scale_p.h>

#define SLOPE_LINEAR_PRIVATE(instance) ((slope_linear_private_t*) (instance))
#define SLOPE_LINEAR_GET_PRIVATE(instance) SLOPE_LINEAR_PRIVATE(SLOPE_SCALE_GET_PRIVATE(instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_linear_private slope_linear_private_t;

struct _slope_linear_private {
   slope_scale_private_t parent;

   slope_list_t *axis_list;

   double x_low_bound, x_up_bound;
   double y_low_bound, y_up_bound;
   
   double dat_x_min, dat_x_max;
   double dat_y_min, dat_y_max;
   double dat_width, dat_height;
   
   double fig_x_min, fig_x_max;
   double fig_y_min, fig_y_max;
   double fig_width, fig_height;
};

SLOPE_END_DECLS

#endif /*__SLOPE_LINEAR_P__*/
