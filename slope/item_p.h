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

#ifndef SLOPE_DATA_P_H
#define SLOPE_DATA_P_H

#include "slope/item.h"
#include "slope/text.h"

SLOPE_BEGIN_DECLS


/**
 */
typedef struct _slope_item_class slope_item_class_t;


/**
 */
struct _slope_item_class
{
    void (*destroy_fn) (slope_item_t*);

    void (*draw_fn) (slope_item_t*, cairo_t*, const slope_metrics_t*);

    void (*draw_thumb_fn) (slope_item_t*, const slope_point_t*, cairo_t*);

    void (*get_rect_fn) (slope_item_t*, slope_rect_t*);
};


/**
 */
struct _slope_item
{
    slope_item_class_t *klass;
    slope_metrics_t *metrics;
    char *name;
    slope_font_t *font;
    slope_bool_t visible;
    slope_bool_t rescalable;
    int has_thumb;
};


/**
 */
void _slope_item_draw (slope_item_t *item, cairo_t *cr,
                        const slope_metrics_t *metrics);


/**
 */
void _slope_item_draw_thumb (slope_item_t *item,
                              const slope_point_t *pos, cairo_t *cr);


/**
 */
int _slope_item_parse_color (const char *fmt);


/**
 */
int _slope_item_parse_scatter (const char *fmt);


void
_slope_item_set_metrics (slope_item_t *item, slope_metrics_t *metrics);


SLOPE_END_DECLS

#endif /*SLOPE_DATA_P_H */
