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

#ifndef SLP_TREE_H
#define SLP_TREE_H

#include "slope/list.h"

typedef struct _SlpTree SlpTree;
#define SLP_TREE(Addr) ((SlpTree*) (Addr))

struct _SlpTree {
    SlpList siblings;
    SlpList children;
};

#define SLP_TREE_INIT(T) \
    G_STMT_START { \
        SLP_LIST_INIT(&SLP_TREE(T)->siblings); \
        SLP_LIST_INIT(&SLP_TREE(T)->children); \
    } G_STMT_END

#endif /* SLP_TREE_H */
