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

#ifndef _slope_item_p_h_
#define _slope_item_p_h_

#include "slope/item.h"
#include "slope/tree.h"
#include "slope/text.h"

typedef struct _SlopeItemPrivate SlopeItemPrivate;
#define SLOPE_ITEM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_ITEM, SlopeItemPrivate))

struct _SlopeItemPrivate
{
    SlopeTree tree_node;
    SlopeItem *publ_obj;
    SlopeFigure *figure;
    SlopeText *text;
};


void draw_item_p (SlopeItemPrivate *m, cairo_t *cr, const SlopeRect *rec);


#endif /* _slope_item_p_h_ */
