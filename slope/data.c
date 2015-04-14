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

#include "slope/data_p.h"
#include "slope/metrics.h"
#include <stdlib.h>


void slope_data_destroy (slope_data_t *data)
{
    if (data == NULL) {
        return;
    }
    if (data->klass->destroy_fn) {
        (*data->klass->destroy_fn)(data);
    }
    if (data->name) {
        free(data->name);
    }
    free(data);
}


int slope_data_get_visible (slope_data_t *data)
{
    if (data == NULL) {
        return SLOPE_FALSE;
    }
    return data->visible;
}


void slope_data_set_visible (slope_data_t *data,
                             int visible)
{
    if (data == NULL) {
        return;
    }
    data->visible = visible;
    if (data->appearence_change_callback) {
        (*data->appearence_change_callback)(data);
    }
}


void __slope_data_draw (slope_data_t *data, cairo_t *cr,
                        const slope_metrics_t *metrics)
{
    (*data->klass->draw_fn)(data, cr, metrics);
}


slope_metrics_t* slope_data_get_metrics (slope_data_t *data)
{
    if (data == NULL) {
        return NULL;
    }
    return data->metrics;
}


slope_scene_t* slope_data_get_scene (slope_data_t *data)
{
    if (data == NULL) {
        return NULL;
    }
    return slope_metrics_get_scene(data->metrics);
}

/* slope/data.c */
