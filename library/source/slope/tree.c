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

#include "slope/tree.h"

SlopeTree*
slope_tree_append (SlopeTree *parent, SlopeTree *child)
{
    child->next = NULL;
    child->prev = parent->last;

    if (parent->first != NULL) {
        parent->last->next = child;
    } else {
        parent->first = child;
    }

    parent->last = child;
    return child;
}


SlopeTree*
slope_tree_prepend (SlopeTree *parent, SlopeTree *child)
{
    child->prev = NULL;
    child->next = parent->first;

    if (parent->first != NULL) {
        parent->first->prev = child;
    } else {
        parent->last = child;
    }

    parent->first = child;
    return child;
}

/* slope/tree.c */
