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

#ifndef __SLOPE_SERIES__
#define __SLOPE_SERIES__

#include <slope/item.h>

#define SLOPE_SERIES(instance) ((slope_series_t*) (instance))
#define SLOPE_SERIES_CLASS(instance) ((slope_series_class_t*) (instance))
#define SLOPE_SERIES_GET_CLASS(instance) SLOPE_SERIES_CLASS(SLOPE_ITEM_GET_CLASS(instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_series slope_series_t;
typedef struct _slope_series_class slope_series_class_t;

struct _slope_series_class {
   slope_item_class_t parent;
};


struct _slope_series {
   slope_item_t parent;
};


typedef enum _slope_series_style
{
    SLOPE_SERIES_LINE,
    SLOPE_SERIES_CIRCLES,
    SLOPE_SERIES_AREAUNDER
}
slope_series_symbol_t;


slope_item_t* slope_series_new_empty (const char *name);

slope_item_t* slope_series_new (const double *vx, const double *vy,
                                int size, const char *name, const char *style);

void slope_series_set_style (slope_item_t *self, const char *style);

void slope_series_init (slope_item_t *self);

void slope_series_finalize (slope_item_t *self);

void slope_series_set_data (slope_item_t *self,
                            const double *vx, const double *vy,
                            int size, const char *style);

void slope_series_set_symbol (slope_item_t *self, int symbol);

void slope_series_set_radius (slope_item_t *self, double radius);

SLOPE_END_DECLS

#endif /*__SLOPE_SERIES__*/
