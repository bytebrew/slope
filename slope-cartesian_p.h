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

#ifndef _SLOPE_CARTESIAN_P_H_
#define _SLOPE_CARTESIAN_P_H_

#include "slope-cartesian.h"
#include "slope-plotable_p.h"
#include "slope-list.h"

#ifdef __cplusplus
extern "C" {
#endif
    
struct _slope_cartesian
{
    struct _slope_plotable base;
    slope_list_t *scatters;
};

void _slope_cartesian_cleanup (slope_plotable_t *cartesian);
void _slope_cartesian_draw (slope_plotable_t *cartesian_base, cairo_t *cr, slope_rect_t *scene_rect);
    
#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_CARTESIAN_P_H_*/