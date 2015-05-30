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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_LEGEND_P_H
#define __SLOPE_LEGEND_P_H

#include "slope/legend.h"
#include "slope/item_p.h"

__SLOPE_BEGIN_DECLS

typedef struct _slope_legend slope_legend_t;

struct _slope_legend
{
    slope_item_t parent;
    slope_rect_t rect;
    slope_color_t fill_color;
    slope_color_t stroke_color;
    slope_legend_position_t position;
};

/**
 */
slope_item_class_t* __slope_legend_get_class();


/**
 */
void __slope_legend_draw (slope_item_t *legend, cairo_t *cr,
                          const slope_metrics_t *metrics);

/**
 */
void __slope_legend_eval_geometry (slope_item_t *legend,
                                   const slope_metrics_t *metrics);

__SLOPE_END_DECLS

#endif /*__SLOPE_LEGEND_P_H */

