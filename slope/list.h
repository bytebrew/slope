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

 /**
  * @file slope/list.h
  *
  * @defgroup List
  * @ingroup List
  * @author Elvis Teixeira
  * @date 18 Jan 2015
  *
  * @brief Functions to create and manipulate linked lists of pointers
  * Used to store and iterate over linked lists of pointers to slope's
  * objects.
  */

#ifndef SLOPE_LIST_H
#define SLOPE_LIST_H

#include "slope/global.h"

SLOPE_BEGIN_DECLS

typedef struct _slope_iterator slope_iterator_t;

typedef struct _slope_list slope_list_t;

/**
 * @ingroup List
 * @brief Access the data pointed to by iter
 * @return an untyped pointer to the data pointed to by iter
 */
slope_public void*
slope_iterator_data (const slope_iterator_t *iter);

/**
 * @ingroup List
 * @brief Moves the iterator to the next position
 */
slope_public void
slope_iterator_next (slope_iterator_t **iter);

/**
 * @ingroup List
 * @brief Moves the iterator to the previous position
 */
slope_public void
slope_iterator_previous (slope_iterator_t **iter);

/**
 * @ingroup List
 * @brief Appends an element to the end of the list
 * @return the newly allocated list
 */
slope_public slope_list_t*
slope_list_append (slope_list_t *list, void *data);

/**
 * @ingroup List
 * @brief Prepends an element to the begining of the list
 * @return the newly allocated list
 */
slope_public slope_list_t*
slope_list_prepend (slope_list_t *list, void *data);

/**
 * @ingroup List
 * @brief Destroys list
 */
slope_public void
slope_list_destroy (slope_list_t *list);

/**
 * @ingroup List
 * @brief Access the iterator for the first element
 * @return an iterator pointing to the first element
 */
slope_public slope_iterator_t*
slope_list_first (const slope_list_t *list);

/**
 * @ingroup List
 * @brief Access the iterator for the last element
 * @return an iterator pointing to the last element
 */
slope_public slope_iterator_t*
slope_list_last (const slope_list_t *list);

/**
 * @ingroup List
 * @brief Access the size (element number) of the last
 * @return the size of the string
 */
slope_public int
slope_list_size (const slope_list_t *list);

/**
 * @ingroup List
 * @brief Removes the element pointed to by iterator pos
 * @return the next valid element, NULL the list ended
 */
slope_public slope_iterator_t*
slope_list_remove (slope_list_t *list,
                   slope_iterator_t *pos);

SLOPE_END_DECLS

#endif /*SLOPE_LIST_H */
