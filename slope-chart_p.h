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

#ifndef _SLOPE_CHART_P_H_
#define _SLOPE_CHART_P_H_

#include "slope-chart.h"
#include "slope-list.h"
#include "slope-primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_chart
{
    slope_list_t *plotables;
    slope_color_t back_color;
    int fill_back;
    double x_low_b, x_up_b;
    double y_low_b, y_up_b;
};

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_CHART_P_H_*/