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

#ifndef __SLOPE_SCENE_P_H
#define __SLOPE_SCENE_P_H

#include "slope/figure.h"

__SLOPE_BEGIN_DECLS

/**
 */
struct _slope_figure
{
    slope_list_t    *metrics;
    slope_metrics_t *default_metrics;
    slope_color_t    back_color;
    int              fill_back;
    slope_callback_t change_callback;
};

__SLOPE_END_DECLS

#endif /*__SLOPE_SCENE_P_H */
