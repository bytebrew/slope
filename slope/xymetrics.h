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

#ifndef __SLOPE_XYMETRICS_H
#define __SLOPE_XYMETRICS_H

#include "slope/metrics.h"
#include "slope/xyaxis.h"

__SLOPE_BEGIN_DECLS

/**
 */
slope_public slope_metrics_t* slope_xymetrics_create();

/**
 */
slope_public double
slope_xymetrics_map_x (const slope_metrics_t *metrics, double x);

/**
 */
slope_public double
slope_xymetrics_map_y (const slope_metrics_t *metrics, double y);

/**
 */
slope_public slope_data_t*
slope_xymetrics_get_axis (slope_metrics_t *metrics,
                          slope_xyaxis_type_t type);

/**
 */
slope_public void
slope_xymetrics_set_x_boundary (slope_metrics_t *metrics,
                                double low, double hi);

/**
 */
slope_public void
slope_xymetrics_set_y_boundary (slope_metrics_t *metrics,
                                double low, double hi);

/**
 */
slope_public void
slope_xymetrics_set_x_range (slope_metrics_t *metrics,
                             double xi, double xf);

/**
 */
slope_public void
slope_xymetrics_set_y_range (slope_metrics_t *metrics,
                             double yi, double yf);

__SLOPE_END_DECLS

#endif /*__SLOPE_XYMETRICS_H */
