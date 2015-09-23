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

#ifndef SLOPE_FIGURE_P_H
#define SLOPE_FIGURE_P_H

#include "slope/figure.h"


#define SLOPE_FIGURE_GET_PRIVATE(figure) \
    ((slope_figure_private_t*) ((slope_object_t*) (figure))->priv)


SLOPE_BEGIN_DECLS

typedef struct _slope_figure_private slope_figure_private_t;

struct _slope_figure_private
{
    slope_list_t     *metrics;
    slope_metrics_t  *default_metrics;
    slope_font_t     *default_font;
    slope_item_t     *legend;
    slope_callback_t  change_callback;
    slope_color_t     back_color;
    int               fill_back;
};


void
_slope_figure_destroy (slope_object_t *self);

SLOPE_END_DECLS

#endif /*SLOPE_FIGURE_P_H */
