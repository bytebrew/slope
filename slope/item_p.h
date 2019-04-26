/*
 * Copyright (C) 2019  Elvis Teixeira
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

#ifndef SLOPE_ITEM_P_H
#define SLOPE_ITEM_P_H

#if !defined (SLOPE_COMPILATION)
#error "You should not include Slope internal headers."
#endif

#include <slope/item.h>
#include <slope/tree.h>

typedef enum {
    ITEM_VISIBLE
} ItemFlag;


typedef struct {
    SlopeTree tree_node;
    SlopeItem *item;
    ItemFlag flags;
} SlopeItemPrivate;

#endif /* SLOPE_ITEME_P_H */
