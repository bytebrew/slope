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

#ifndef __SLOPE_AXIS__
#define __SLOPE_AXIS__

#include <slope/item.h>
#include <slope/sampler.h>

#define SLOPE_AXIS(instance) ((slope_axis_t*) (instance))
#define SLOPE_AXIS_CLASS(instance) ((slope_axis_class_t*) (instance))
#define SLOPE_AXIS_GET_CLASS(instance) SLOPE_AXIS_CLASS(SLOPE_ITEM_GET_CLASS(instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_axis slope_axis_t;
typedef struct _slope_axis_class slope_axis_class_t;

struct _slope_axis_class {
   slope_item_class_t parent;
};


struct _slope_axis {
   slope_item_t parent;
};


typedef enum _slope_axis_position
{
    SLOPE_AXIS_BOTTOM = 0x00000001,
    SLOPE_AXIS_TOP    = 0x00000002,
    SLOPE_AXIS_LEFT   = 0x00000004,
    SLOPE_AXIS_RIGHT  = 0x00000008
}
slope_axis_position_t;


typedef enum _slope_axis_element
{
    SLOPE_AXIS_LINE   = 0x00000001,
    SLOPE_AXIS_TICKS  = 0x00000002,
    SLOPE_AXIS_GRID   = 0x00000004,
    SLOPE_AXIS_TITLE  = 0x00000008,
    SLOPE_AXIS_ALL    = 0xFFFFFFFF
}
slope_axis_element_t;


slope_item_t* slope_axis_new (slope_scale_t *linear_scale, const char *name, slope_axis_position_t pos);

void slope_axis_init (slope_item_t *self);
void slope_axis_finalize (slope_item_t *self);

slope_sampler_t* slope_axis_get_sampler (const slope_item_t *self);
slope_axis_position_t slope_axis_get_position (const slope_item_t *self);
slope_axis_element_t slope_axis_get_elements (const slope_item_t *self);
void slope_axis_set_elements (slope_item_t *self, int elements);
void slope_axis_set_colors (slope_item_t *self, slope_color_t stroke_color, slope_color_t extra_color);

SLOPE_END_DECLS

#endif /*__SLOPE_AXIS__*/
