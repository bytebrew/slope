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

#ifndef _SLOPE_METRICS_H_
#define _SLOPE_METRICS_H_

#include "slope/primitives.h"
#include "slope/legend.h"
#include "slope/data.h"
#include "slope/list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _slope_metrics slope_metrics_t;


void slope_metrics_destroy (slope_metrics_t *metrics);

void slope_metrics_add_data (slope_metrics_t *metrics,
                             slope_data_t *data);

slope_list_t* slope_metrics_data_list (slope_metrics_t *metrics);

int slope_metrics_visible  (slope_metrics_t *metrics);

void slope_metrics_update (slope_metrics_t *metrics);

slope_legend_t* slope_metrics_legend (slope_metrics_t *metrics);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_METRICS_H_*/
