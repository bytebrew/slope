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

#ifndef SLOPE_XYMETRICS_P_H
#define SLOPE_XYMETRICS_P_H

#include "slope/xymetrics.h"
#include "slope/metrics_p.h"


#define SLOPE_XYMETRICS_GET_PRIVATE(metrics) \
    ((slope_xymetrics_private_t*) ((slope_object_t*) (metrics))->priv)


SLOPE_BEGIN_DECLS


typedef struct _slope_xymetrics slope_xymetrics_t;
typedef struct _slope_xymetrics_private slope_xymetrics_private_t;


struct _slope_xymetrics_private
{
    slope_metrics_private_t parent;
    
    /* axis list */
    slope_list_t *axis_list;
    
    /* item space geometry attributes */
    double xmin, xmax;
    double ymin, ymax;
    double width, height;
};


slope_metrics_class_t*
_slope_xymetrics_get_class();


void
_slope_xymetrics_init (slope_object_t *object);


void
_slope_xymetrics_destroy (slope_object_t *metrics);


void
_slope_xymetrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                       const slope_rect_t *rect);


void
_slope_xymetrics_update (slope_metrics_t *metrics);


void
_slope_xymetrics_get_data_rect (const slope_metrics_t *metrics,
                                slope_rect_t *rect);

SLOPE_END_DECLS

#endif /*SLOPE_XYMETRICS_P_H */
