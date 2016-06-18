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

#ifndef __SLOPE_CHART__
#define __SLOPE_CHART__

#include <slope/widget.h>
#include <slope/figure.h>
#include <slope/linear.h>
#include <slope/axis.h>
#include <slope/legend.h>
#include <slope/series.h>

SLOPE_BEGIN_DECLS

typedef struct _slope_chart slope_chart_t;


slope_chart_t* slope_chart_new (const char *title);

void slope_chart_plot (slope_chart_t *self, const double *vx, const double *vy,
                       int size, const char *name, const char *style);

void slope_chart_show (slope_chart_t *self);

void slope_chart_destroy (slope_chart_t *self);

slope_figure_t* slope_chart_get_figure (slope_chart_t *self);

slope_item_t* slope_chart_get_axis (slope_chart_t *self, slope_axis_position_t pos);

SLOPE_END_DECLS

#endif /*__SLOPE_CHART__*/
