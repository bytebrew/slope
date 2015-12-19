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
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_LIST__
#define __SLOPE_LIST__

#include <slope/global.h>

#define SLOPE_LIST_FOREACH(iter, list) \
   for (iter = slope_list_first(list); \
        iter != NULL; \
        slope_iterator_next(&iter))

SLOPE_BEGIN_DECLS

typedef struct _slope_list slope_list_t;
typedef struct _slope_iterator slope_iterator_t;

slope_list_t* slope_list_new ();
void slope_list_destroy (slope_list_t *self);
void slope_list_append (slope_list_t *self, void *data);
void slope_list_prepend (slope_list_t *self, void *data);
slope_iterator_t* slope_list_first (slope_list_t *self);
slope_iterator_t* slope_list_last (slope_list_t *self);
int slope_list_size (const slope_list_t *self);
slope_bool_t slope_list_contains (const slope_list_t *self, void *element);
void* slope_iterator_data (slope_iterator_t *self);
void slope_iterator_next (slope_iterator_t **self);
void slope_iterator_previous (slope_iterator_t **self);
void slope_list_clear (slope_list_t *self);

SLOPE_END_DECLS

#endif /*__SLOPE_LIST__*/
