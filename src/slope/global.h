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

#ifndef __SLOPE_GLOBAL__
#define __SLOPE_GLOBAL__

#ifdef __cplusplus
# define SLOPE_BEGIN_DECLS extern "C" {
# define SLOPE_END_DECLS } /* extern "C" */
#else
# define SLOPE_BEGIN_DECLS /* nothing */
# define SLOPE_END_DECLS /* nothing */
#endif

typedef struct _slope_figure slope_figure_t;
typedef struct _slope_figure_class slope_figure_class_t;

typedef struct _slope_scale slope_scale_t;
typedef struct _slope_scale_class slope_scale_class_t;

typedef struct _slope_item  slope_item_t;
typedef struct _slope_item_class  slope_item_class_t;

#define SLOPE_ALLOC_BYTES(amount) (malloc ((size_t) amount))
#define SLOPE_ALLOC(type) ((type*) SLOPE_ALLOC_BYTES(sizeof(type)))
#define SLOPE_FREE(ptr) (free(ptr))

typedef int slope_bool_t;
#define SLOPE_FALSE 0
#define SLOPE_TRUE  1

#endif /*__SLOPE_GLOBAL__*/
