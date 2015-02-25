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

#ifndef _SLOPE_SCALE_P_H_
#define _SLOPE_SCALE_P_H_

#ifdef __cplusplus
extern "C" {
#endif

struct _slope_scale_t
{
    slope_chart_t *chart;
    slope_list_t *plotables;
    slope_rect_t *chart_rect;
    void (*_update_fn)(struct _slope_scale*);
};

void _slope_scale_set_chart_rect (slope_scale_t *scale, slope_rect_t *rect);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_SCALE_P_H_*/
