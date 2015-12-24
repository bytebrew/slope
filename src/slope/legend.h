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

#ifndef __SLOPE_LEGEND__
#define __SLOPE_LEGEND__

#include <slope/item.h>

#define SLOPE_LEGEND(instance) ((slope_legend_t*) (instance))
#define SLOPE_LEGEND_CLASS(instance) ((slope_legend_class_t*) (instance))
#define SLOPE_LEGEND_GET_CLASS(instance) SLOPE_LEGEND_CLASS(SLOPE_ITEM_GET_CLASS(instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_legend slope_legend_t;
typedef struct _slope_legend_class slope_legend_class_t;

struct _slope_legend_class {
   slope_item_class_t parent;
};


struct _slope_legend {
   slope_item_t parent;
};


typedef enum _slope_legend_position_policy
{
    SLOPE_LEGEND_TOPLEFT,
    SLOPE_LEGEND_TOPRIGHT,
    SLOPE_LEGEND_BOTTOMLEFT,
    SLOPE_LEGEND_BOTTOMRIGHT,
    SLOPE_LEGEND_CUSTOMPOS
}
slope_legend_position_policy_t;


slope_item_t* slope_legend_new(slope_figure_t *figure);

void slope_legend_init (slope_item_t *self);
void slope_legend_finalize (slope_item_t *self);

void slope_legend_set_position_policy (slope_item_t *self, slope_legend_position_policy_t policy);
void slope_legend_set_position (slope_item_t *self, double x, double y);
void slope_legend_set_colors (slope_item_t *self, slope_color_t stroke_color, slope_color_t fill_color);

SLOPE_END_DECLS

#endif /*__SLOPE_LEGEND__*/