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

#ifndef _SLOPE_METRICS_P_H_
#define _SLOPE_METRICS_P_H_

#include "slope/primitives.h"
#include "slope/metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_metrics
{
    slope_list_t *data;
    int visible;

    void (*_cleanup_fn) (slope_metrics_t*);
    void (*_update_fn) (slope_metrics_t*);
    void (*_draw_fn) (slope_metrics_t*, cairo_t*, slope_rect_t*);
};

void _slope_metrics_draw  (slope_metrics_t *metrics,
                           cairo_t *cr, slope_rect_t *area);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_METRICS_P_H_*/
