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
#include <stdlib.h>


void slope_data_destroy (slope_data_t *data)
{
    if (data == NULL) {
        return;
    }
    if (data->_cleanup_fn) {
        (*data->_cleanup_fn)(data);
    }
    if (data->name) {
        free(data->name);
    }
    free(data);
    data = NULL;
}


int slope_data_visible (slope_data_t *data)
{
    if (data == NULL) {
        return 0;
    }
    return data->visible;
}


const char* slope_data_name (slope_data_t *data)
{
    if (data == NULL) {
        return NULL;
    }
    return data->name;
}


void _slope_data_draw (slope_data_t *data, cairo_t *cr,
                       slope_metrics_t *metrics)
{
    if (data->visible) {
        (*data->_draw_fn)(data,cr,metrics);
    }
}

/* slope/data.c */
