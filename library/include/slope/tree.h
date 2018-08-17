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

#ifndef _slope_tree_h_
#define _slope_tree_h_

#include "slope/global.h"

typedef struct _SlopeTree SlopeTree;
#define SLOPE_TREE(Addr) ((SlopeTree *) (Addr))

struct _SlopeTree {
    SlopeTree *parent;
    SlopeTree *prev;
    SlopeTree *next;
    SlopeTree *first;
    SlopeTree *last;
};

#define slope_tree_init(Addr) \
    G_STMT_START { \
        SLOPE_TREE(Addr)->parent = NULL; \
        SLOPE_TREE(Addr)->prev = NULL; \
        SLOPE_TREE(Addr)->next = NULL; \
        SLOPE_TREE(Addr)->first = NULL; \
        SLOPE_TREE(Addr)->last = NULL; \
    } G_STMT_END


SlopeTree* slope_tree_append (SlopeTree *parent, SlopeTree *child);

SlopeTree* slope_tree_prepend (SlopeTree *parent, SlopeTree *child);

void slope_tree_destroy (SlopeTree *root, SlopeCB cleanup);

#endif /* _slope_tree_h_ */
