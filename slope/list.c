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

#include "slope/list.h"
#include <stdlib.h>


struct _slope_iterator
{
    struct _slope_iterator *next;
    struct _slope_iterator *prev;
    void *item;
};


struct _slope_list
{
    struct _slope_iterator *first;
    struct _slope_iterator *last;
    int size;
};


/*
 * Access the item pointed to by iter
 */
void* slope_iterator_item (slope_iterator_t *iter)
{
    return iter->item;
}

/*
 * Moves the iterator to the next position
 */
void slope_iterator_next (slope_iterator_t **iter)
{
    *iter = (*iter)->next;
}

/*
 * Moves the iterator to the previous position
 */
void slope_iterator_previous (slope_iterator_t **iter)
{
    *iter = (*iter)->prev;
}

/*
 * Appends an element to the end of the list
 */
slope_list_t* slope_list_append (slope_list_t *list, void *item)
{
    slope_iterator_t *iter =
        malloc(sizeof(slope_iterator_t));
    iter->prev = NULL;
    iter->next = NULL;
    iter->item = item;
    if (list == NULL) {
        list = malloc(sizeof(slope_list_t));
        list->first = iter;
        list->last = iter;
        list->size = 1;
    }
    else {
        list->last->next = iter;
        iter->prev = list->last;
        list->last = iter;
        list->size += 1;
    }
    return list;
}

/*
 * Prepends an element to the begining of the list
 */
slope_list_t* slope_list_prepend (slope_list_t *list, void *item)
{
    slope_iterator_t *iter =
        malloc(sizeof(slope_iterator_t));
    iter->prev = NULL;
    iter->next = NULL;
    iter->item = item;
    if (list == NULL) {
        list = malloc(sizeof(slope_list_t));
        list->first = iter;
        list->last = iter;
        list->size = 1;
    }
    else {
        list->first->prev = iter;
        iter->next = list->first;
        list->first = iter;
        list->size += 1;
    }
    return list;
}

/*
 * Destroys list
 */
void slope_list_destroy (slope_list_t *list)
{
    if (list == NULL) {
        return;
    }
    slope_iterator_t *iter = list->first;
    while (iter) {
        slope_iterator_t *next = iter->next;
        free(iter);
        iter = next;
    }
    free(list);
}

/*
 * Access the iterator for the first element
 */
slope_iterator_t* slope_list_first (slope_list_t *list)
{
    if (list == NULL) {
        return NULL;
    }
    return list->first;
}

/*
 * Access the iterator for the last element
 */
slope_iterator_t* slope_list_last (slope_list_t *list)
{
    if (list == NULL) {
        return NULL;
    }
    return list->last;
}

/**
 * Access the size (element number) of the last
 */
int slope_list_size (slope_list_t *list)
{
    if (list == NULL) {
        return 0;
    }
    return list->size;
}

/**
 * Removes the element pointed to by iterator pos
 */
slope_iterator_t* slope_list_remove (slope_list_t *list,
                                     slope_iterator_t *pos)
{
    slope_iterator_t *next = pos->next;
    slope_iterator_t *prev = pos->prev;
    free(pos);
    list->size -= 1;
    if (prev == NULL) {
        if (next == NULL) {
            list->first = NULL;
            list->last = NULL;
            return NULL;
        }
        list->first = next;
        next->prev = NULL;
        return next;
    }
    if (next == NULL) {
        list->last = prev;
        prev->next = NULL;
        return NULL;
    }
    next->prev = prev;
    prev->next = next;
    return next;
}

/* slope/list.c */
