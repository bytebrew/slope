/*
 * Copyright (C) 2018  Elvis Teixeira
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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _slope_axis2d_p_h_
#define _slope_axis2d_p_h_

#include "slope/axis2d.h"
#include "slope/item_p.h"

typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))


struct _SlopeAxis2DPrivate
{
    double data_margin;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;
    double fig_width, fig_height;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
    double dat_width, dat_height;
};


#define SLOPE_AXIS2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))

#endif /* _slope_axis2d_p_h_ */
