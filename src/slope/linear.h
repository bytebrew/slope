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

#ifndef __SLOPE_LINAR__
#define __SLOPE_LINEAR__

#include <slope/scale.h>
#include <slope/axis.h>

#define SLOPE_LINEAR(instance) ((slope_linear_t*) (instance))
#define SLOPE_LINEAR_CLASS(instance) ((slope_linear_class_t*) (instance))
#define SLOPE_LINEAR_GET_CLASS(instance) SLOPE_LINEAR_CLASS(SLOPE_SCALE_GET_CLASS(instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_linear slope_linear_t;
typedef struct _slope_linear_class slope_linear_class_t;

struct _slope_linear_class {
   slope_scale_class_t parent;
};


struct _slope_linear {
   slope_scale_t parent;
};


slope_scale_t* slope_linear_new (const char *name);

void slope_linear_init (slope_scale_t *self);
void slope_linear_finalize (slope_scale_t *self);

slope_item_t* slope_linear_get_axis (slope_scale_t *self, slope_axis_position_t pos);
void slope_linear_set_show_grid (slope_scale_t *self, slope_bool_t show);
void slope_linear_set_x_boundaries (slope_scale_t *self, double low_bound, double up_bound);
void slope_linear_set_y_boundaries (slope_scale_t *self, double low_bound, double up_bound);
void slope_linear_set_axis_labels (slope_scale_t *self, const char *bottom_label, const char *left_label,
                                   const char *top_label, const char *right_label);

SLOPE_END_DECLS

#endif /*__SLOPE_LINEAR__*/
