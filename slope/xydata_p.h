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

#ifndef __SLOPE_XYDATA_P_H
#define __SLOPE_XYDATA_P_H

#include "slope/xydata.h"
#include "slope/data_p.h"

__SLOPE_BEGIN_DECLS

typedef struct _slope_xydata slope_xydata_t;

struct _slope_xydata
{
    slope_data_t    parent;
    int             rescalable;
    const double   *vx, *vy;
    int             n;
    double          xmin, xmax;
    double          ymin, ymax;
    slope_color_t   color;
    slope_scatter_t scatter;
    int             fill_symbol;
    int             antialias;
    double          line_width;
};

/**
 */
slope_data_class_t* __slope_xydata_get_class();

void __slope_xydata_init (slope_data_t *data);

/**
 */
void __slope_xydata_draw (slope_data_t *data, cairo_t *cr,
                          const slope_metrics_t *metrics);

/**
 */
void __slope_xydata_draw_line (slope_data_t *data, cairo_t *cr,
                               const slope_metrics_t *metrics);

/**
 */
void __slope_xydata_draw_circles (slope_data_t *data, cairo_t *cr,
                                  const slope_metrics_t *metrics);

/**
 */
void __slope_xydata_draw_triangles (slope_data_t *data, cairo_t *cr,
                                    const slope_metrics_t *metrics);

/**
 */
void __slope_xydata_draw_squares (slope_data_t *data, cairo_t *cr,
                                  const slope_metrics_t *metrics);

/**
 */
void __slope_xydata_draw_plusses (slope_data_t *data, cairo_t *cr,
                                  const slope_metrics_t *metrics);

/**
 */
void __slope_xydata_check_ranges (slope_data_t *data);

__SLOPE_END_DECLS

#endif /*__SLOPE_XYDATA_P_H */
