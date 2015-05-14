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

#ifndef __SLOPE_DATA_P_H
#define __SLOPE_DATA_P_H

#include "slope/item.h"

__SLOPE_BEGIN_DECLS

/**
 */
typedef struct _slope_item_class slope_item_class_t;

/**
 */
struct _slope_item_class
{
    void (*destroy_fn) (slope_item_t*);

    void (*draw_fn) (slope_item_t*, cairo_t*,
                     const slope_metrics_t*);
};

/**
 */
struct _slope_item
{
    slope_item_class_t *klass;
    slope_metrics_t *metrics;
    char *name;
    int visible;
    int has_thumb;
};

/**
 */
void __slope_item_draw (slope_item_t *item, cairo_t *cr,
                        const slope_metrics_t *metrics);

/**
 */
int __slope_item_parse_color (const char *fmt);

/**
 */
int __slope_item_parse_scatter (const char *fmt);

__SLOPE_END_DECLS

#endif /*__SLOPE_DATA_P_H */

