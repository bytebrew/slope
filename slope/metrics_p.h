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

#ifndef SLOPE_METRICS_P_H
#define SLOPE_METRICS_P_H

#include "slope/metrics.h"


#define SLOPE_METRICS_GET_PRIVATE(metrics) \
    ((slope_metrics_private_t*) ((slope_object_t*) (metrics))->priv)

#define SLOPE_METRICS_GET_CLASS(metrics) \
    ((slope_metrics_class_t*) ((slope_object_t*) (metrics))->klass)


SLOPE_BEGIN_DECLS


typedef struct _slope_metrics_class slope_metrics_class_t;
typedef struct _slope_metrics_private slope_metrics_private_t;


struct _slope_metrics_class
{
    slope_object_class_t parent;
    void (*update_fn) (slope_metrics_t*);
    void (*draw_fn) (slope_metrics_t*, cairo_t*, const slope_rect_t*);
    void (*get_data_rect_fn) (const slope_metrics_t*, slope_rect_t*);
};


struct _slope_metrics_private
{
    slope_metrics_class_t *klass;
    slope_metrics_type_t   type;
    slope_figure_t        *figure;
    slope_list_t          *item_list;
    
    /* boundary between item image and figure frontier */
    double x_low_bound, x_up_bound;
    double y_low_bound, y_up_bound;
    
    /* figure geometry attributes */
    double xmin_figure, xmax_figure;
    double ymin_figure, ymax_figure;
    double width_figure, height_figure;
    
    /* show this metric's items? */
    slope_bool_t visible;
};


void
_slope_metrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                     const slope_rect_t *rect);


void
_slope_metrics_set_figure (slope_metrics_t *metrics,
                           slope_figure_t *figure);

SLOPE_END_DECLS

#endif /*SLOPE_METRICS_P_H */