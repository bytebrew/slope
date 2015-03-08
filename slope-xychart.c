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

#include "slope-xychart.h"
#include "slope-scene_p.h"

static void _slope_xychart_cleanup (slope_scene_t *chart);


slope_scene_t* slope_xychart_create ()
{
    slope_scene_t *scene = slope_scene_create();
    slope_metrics_t *cartesian = slope_cartesian_create();
    slope_scene_add_metrics(scene, cartesian);
    scene->_cleanup_fn = _slope_xychart_cleanup;
}


static void _slope_xychart_cleanup (slope_scene_t *chart)
{
    /* destroy metrics */
    slope_list_t *met_list = slope_scene_metrics_list(chart);
    slope_iterator_t *met_iter = slope_list_first(met_list);
    while (met_iter) {
        slope_metrics_t *cartesian =
            (slope_metrics_t*) slope_iterator_data(met_iter);
        /* destroy plots */
        slope_list_t *plot_list = slope_cartesian_plot_list(cartesian);
        slope_iterator_t *plot_iter = slope_list_first(plot_list);
        while (plot_iter) {
            slope_xyplot_t *plot =
                (slope_xyplot_t*) slope_iterator_data(plot_iter);
            slope_xyplot_destroy(plot);
            slope_iterator_next(&plot_iter);
        }
        slope_metrics_destroy(cartesian);
        slope_iterator_next(&met_iter);
    }
}


void slope_xychart_add_plot (slope_scene_t *chart,
                             double *vx, double *vy, unsigned long n,
                             slope_color_name_t color,
                             slope_xyplot_symbol_t symbol)
{
    slope_list_t *met_list = slope_scene_metrics_list(chart);
    slope_iterator_t *met_iter = slope_list_first(met_list);
    slope_metrics_t *cartesian =
        (slope_metrics_t*) slope_iterator_data(met_iter);
    slope_xyplot_t *plot =
        slope_xyplot_create_simple(vx,vy,n,color,symbol);
    slope_cartesian_add_xyplot(cartesian,plot);
}

/* slope-xychart.c */