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

#include "slope-scale-cartesian.h"
#include "slope-scale-cartesian_p.h"
#include <stdlib.h>
#include <string.h>


slope_scale_t* slope_scale_cartesian_create (slope_chart_t *chart, const char *name)
{
    slope_scale_t *scale = malloc(sizeof(slope_scale_cartesian_t));
    scale->chart = chart;
    scale->name = strdup(name);
    scale->_update_fn = _slope_scale_cartesian_rescale;
    scale->_cleanup_fn = _slope_scale_cartesian_cleanup;
}

void _slope_scale_cartesian_cleanup (slope_scale_t *scale)
{
    free(scale->name);
}

double slope_scale_cartesian_map_x1 (slope_scale_t *scale, double x)
{
    slope_rect_t *rec = &scale->chart_rect;
    slope_scale_cartesian_t *cscl = (slope_scale_cartesian_t*) scale;
    double ret = (x - cscl->x_min)/ cscl->width;
    return rec->x + ret*rec->width;
}

double slope_scale_cartesian_map_x2 (slope_scale_t *scale, double x)
{
    slope_rect_t *rec = &scale->chart_rect;
    slope_scale_cartesian_t *cscl = (slope_scale_cartesian_t*) scale;
    double ret = (x - cscl->y_min)/ cscl->height;
    return rec->y + (1.0 - ret)*rec->height;
}

void _slope_scale_cartesian_rescale (slope_scale_t *scale)
{
    if (scale->plotables == NULL) {
        scale->x_min = 0.0;
        scale->x_max = 1.0;
        scale->y_min = 0.0;
        scale->y_max = 1.0;
        return;
    }
    slope_iterator_t *iter = slope_list_first(scale->plotables);
    slope_plotable_cartesian_t *plot =
        (slope_plotable_cartesian_t*) slope_iterator_data(iter);
    scale->x_min = plot->x_min;
    scale->x_max = plot->x_max;
    scale->y_min = plot->y_min;
    scale->y_max = plot->y_max;

}

/* slope-scale-cartesian.c */
