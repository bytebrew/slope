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

#include <slope/list.h>
#include <stdlib.h>

struct _slope_iterator {
   struct _slope_iterator *prev;
   struct _slope_iterator *next;
   void *data;
};


struct _slope_list {
   struct _slope_iterator *first;
   struct _slope_iterator *last;
   int size;
};


slope_list_t* slope_list_new ()
{
   slope_list_t *self = SLOPE_ALLOC(slope_list_t);
   self->first = NULL;
   self->last = NULL;
   self->size = 0;
   return self;
}


void slope_list_destroy (slope_list_t *self)
{
   if (self == NULL)
      return;
   slope_iterator_t *iter = self->first;
   while (iter != NULL) {
      slope_iterator_t *next = iter->next;
      SLOPE_FREE(iter);
      iter = next;
   }
   SLOPE_FREE(self);
}


void slope_list_append (slope_list_t *self, void *data)
{
   slope_iterator_t *iter = SLOPE_ALLOC(slope_iterator_t);
   iter->next = NULL;
   iter->data = data;
   if (self->size == 0) {
      iter->prev = NULL;
      self->first = iter;
      self->last = iter;
   }
   else {
      iter->prev = self->last;
      self->last->next = iter;
      self->last = iter;
   }
   self->size += 1;
}


void slope_list_prepend (slope_list_t *self, void *data)
{
   slope_iterator_t *iter = SLOPE_ALLOC(slope_iterator_t);
   iter->prev = NULL;
   iter->data = data;
   if (self->size == 0) {
      iter->next = NULL;
      self->first = iter;
      self->last = iter;
   }
   else {
      iter->next = self->first;
      self->first->prev = iter;
      self->first = iter;
   }
   self->size += 1;

}


slope_iterator_t* slope_list_first (slope_list_t *self)
{
   if (self == NULL)
      return NULL;
   return self->first;
}


slope_iterator_t* slope_list_last (slope_list_t *self)
{
   if (self == NULL)
      return NULL;
   return self->last;
}


int slope_list_size (const slope_list_t *self)
{
    if (self == NULL)
        return 0;
    return self->size;
}


slope_bool_t slope_list_contains (const slope_list_t *self, void *element)
{
    slope_iterator_t *iter = self->first;
    
    while (iter != NULL) {
        if (iter->data == element)
            return SLOPE_TRUE;
        iter = iter->next;
    }
    return SLOPE_FALSE;
}


void* slope_iterator_data (slope_iterator_t *self)
{
   return self->data;
}


void slope_iterator_next (slope_iterator_t **self)
{
   *self = (*self)->next;
}


void slope_iterator_previous (slope_iterator_t **self)
{
   *self = (*self)->prev;
}


void slope_list_clear (slope_list_t *self)
{
   slope_iterator_t *iter = self->first;
   while (iter != NULL) {
      slope_iterator_t *next = iter->next;
      SLOPE_FREE(iter);
      iter = next;
   }
   self->first = NULL;
   self->last = NULL;
   self->size = 0;
}

/* slope/list.c */
