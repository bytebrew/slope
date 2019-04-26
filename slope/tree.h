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

#ifndef SLOPE_TREE_H
#define SLOPE_TREE_H

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <slope/global.h>

G_BEGIN_DECLS

#define SLOPE_LIST(Addr) ((SlopeList *) (Addr))
typedef struct _SlopeList {
    struct _SlopeList *front;
    struct _SlopeList *back;
} SlopeList;


#define SLOPE_TREE(Addr) ((SlopeTree *) (Addr))
typedef struct _SlopeTree {
    struct _SlopeList parent_node;
    struct _SlopeList children_list;
} SlopeTree;


#define SLOPE_LIST_INIT(list) \
    G_STMT_START { \
        (list)->back = NULL; \
        (list)->front = NULL; \
    } G_STMT_END


#define SLOPE_TREE_INIT(tree) \
    G_STMT_START { \
        SLOPE_LIST_INIT(&(tree)->parent_node); \
        SLOPE_LIST_INIT(&(tree)->children_list); \
    } G_STMT_END


SlopeList* slope_list_append (SlopeList *list, SlopeList *node);

SlopeList* slope_list_detach (SlopeList *list, SlopeList *node);

SlopeTree* slope_tree_append (SlopeTree *tree, SlopeTree *node);

SlopeTree* slope_tree_detach (SlopeTree *tree, SlopeTree *node);

G_END_DECLS

#endif /* SLOPE_TREE_H */
