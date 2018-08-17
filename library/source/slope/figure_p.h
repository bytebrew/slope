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

#ifndef _slope_figure_p_h_
#define _slope_figure_p_h_

#include "slope/figure.h"
#include "slope/text.h"
#include "slope/tree.h"

#define RoundedRect   (1U)


typedef struct _SlopeFigurePrivate SlopeFigurePrivate;
#define SLOPE_FIGURE_PRIVATE(Addr) ((SlopeFigurePrivate*) (Addr))

struct _SlopeFigurePrivate
{
    SlopeRGBA bg_fill_color;
    SlopeRGBA bg_stroke_color;
    double bg_stroke_width;
    SlopeTree *item_tree;
    guint64 options;
    SlopeText *text;

    SlopeRGBA title_color;
    gchar *title;
};


#define SLOPE_FIGURE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_FIGURE, SlopeFigurePrivate))

#endif /* _slope_figure_p_h_ */
