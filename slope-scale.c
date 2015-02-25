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

#include "slope-scale.h"
#include "slope-scale_p.h"

const char* slope_scale_name(slope_scale_t *scale)
{
    if (scale == NULL) {
        return NULL;
    }
    return scale->name;
}

slope_iterator_t* slope_scale_first_plotable (slope_scale_t *scale)
{
    return slope_list_first(scale->plotables);
}

slope_list_t* slope_scale_plotable_list (slope_scale_t *scale)
{
    if (scale == NULL) {
        return NULL;
    }
    return scale->plotables;
}

slope_chart_t* slope_scale_chart (slope_scale_t *scale)
{
    if (scale == NULL) {
        return NULL;
    }
   return scale->chart; 
}

void slope_scale_append_plotable (slope_scale_t *scale, slope_plotable_t *plot)
{
    if (plot == NULL) {
        return;
    }
    scale->plotables = slope_list_append(scale->plotables, plot);
    slope_plotable_set_scale(plot,scale);
    if (scale->_update_fn) {
        (*scale->_update_fn)(scale);
    }
}

void slope_scale_get_chart_rect (slope_scale_t *scale, slope_rect_t *rect)
{
    rect->x = scale->chart_rect.x;
    rect->y = scale->chart_rect.y;
    rect->width = scale->chart_rect.width;
    rect->height = scale->chart_rect.height;
}

void _slope_scale_set_chart_rect (slope_scale_t *scale, slope_rect_t *rect)
{
    scale->chart_rect.x = rect.x;
    scale->chart_rect.y = rect.y;
    scale->chart_rect.width = rect.width;
    scale->chart_rect.height = rect.height;
}

void slope_scale_destroy (slope_scale_t *scale)
{
    if (scale == NULL) {
        return;
    }
    if (scale->_cleanup_fn) {
        (*scale->_cleanup_fn) (scale);
    }
    free(scale);
    scale = NULL;
}

/* slope-scale.c */
