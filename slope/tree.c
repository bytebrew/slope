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

#include <slope/tree.h>


#define SLOPE_LIST_APPEND(list, node) \
    G_STMT_START { \
        (node)->back = NULL; \
        (node)->front = (list)->back; \
        if ((list)->back) { \
            (list)->back->back = node; \
        } else { \
            (list)->front = node; \
        } \
        (list)->back = node; \
    } G_STMT_END


#define SLOPE_LIST_DETACH(list, node) \
    G_STMT_START { \
        /* TODO */ \
    } G_STMT_END


SlopeList*
slope_list_append (SlopeList *list, SlopeList *node) {
    SLOPE_LIST_APPEND(list, node);
    return node;
}


SlopeList*
slope_list_detach (SlopeList *list, SlopeList *node) {
    SLOPE_LIST_DETACH(list, node);
    return node;
}


SlopeTree*
slope_tree_append (SlopeTree *tree, SlopeTree *node) {
    SLOPE_LIST_APPEND(&tree->children_list, &node->parent_node);
    return node;
}


SlopeTree*
slope_tree_detach (SlopeTree *tree, SlopeTree *node) {
    SLOPE_LIST_DETACH(&tree->children_list, &node->parent_node);
    return node;
}
