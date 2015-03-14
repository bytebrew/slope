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

#include "slope/metrics_p.h"
#include "slope/legend_p.h"
#include <stdlib.h>

void slope_metrics_destroy (slope_metrics_t *metrics)
{
    if (metrics == NULL) {
        return;
    }
    if (metrics->_cleanup_fn) {
        (*metrics->_cleanup_fn)(metrics);
    }
    if (metrics->legend) {
        _slope_legend_destroy(metrics->legend);
    }
    slope_list_destroy(metrics->data);
    free(metrics);
    metrics = NULL;
}


void slope_metrics_add_data (slope_metrics_t *metrics,
                             slope_data_t *data)
{
    metrics->data = slope_list_append(metrics->data, data);
    slope_metrics_update(metrics);
}


slope_list_t* slope_metrics_data_list (slope_metrics_t *metrics)
{
    if (metrics == NULL) {
        return NULL;
    }
    return metrics->data;
}


int slope_metrics_visible  (slope_metrics_t *metrics)
{
    if (metrics == NULL) {
        return 0;
    }
    return metrics->visible;
}


void _slope_metrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                          slope_rect_t *area)
{
    if (metrics->visible) {
        (*metrics->_draw_fn)(metrics, cr, area);
    }
}


void slope_metrics_update (slope_metrics_t *metrics)
{
    if (metrics->_update_fn) {
        (*metrics->_update_fn)(metrics);
    }
}


slope_legend_t* slope_metrics_legend (slope_metrics_t *metrics)
{
    if (metrics == NULL) {
        return NULL;
    }
    return metrics->legend;
}

/* slope/metrics.c */
