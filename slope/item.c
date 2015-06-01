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

#include "slope/item_p.h"
#include "slope/metrics.h"
#include "slope/figure.h"
#include <stdlib.h>
#include <string.h>


void slope_item_destroy (slope_item_t *item)
{
    if (item == NULL) {
        return;
    }
    if (item->klass->destroy_fn) {
        (*item->klass->destroy_fn)(item);
    }
    if (item->name) {
        free(item->name);
    }
    free(item);
}


int slope_item_get_visible (const slope_item_t *item)
{
    if (item == NULL) {
        return SLOPE_FALSE;
    }
    return item->visible;
}


void slope_item_set_visible (slope_item_t *item,
                             int visible)
{
    if (item == NULL) {
        return;
    }
    item->visible = visible;
    slope_item_notify_appearence_change(item);
}


const char* slope_item_get_name (const slope_item_t *item)
{
    if (item == NULL) {
        return NULL;
    }
    return item->name;
}


void slope_item_set_name (slope_item_t *item, const char *name)
{
    if (item == NULL) {
        return;
    }
    if (item->name) {
        free(item->name);
    }
    item->name = strdup(name);
    slope_item_notify_appearence_change(item);
}


int slope_item_get_has_thumb (const slope_item_t *item)
{
    if (item == NULL) return SLOPE_FALSE;
    return item->has_thumb;
}


void __slope_item_draw (slope_item_t *item, cairo_t *cr,
                        const slope_metrics_t *metrics)
{
    (*item->klass->draw_fn)(item, cr, metrics);
}


void __slope_item_draw_thumb (slope_item_t *item,
                              const slope_point_t *pos, cairo_t *cr)
{
    (*item->klass->draw_thumb_fn)(item, pos, cr);
}


slope_metrics_t* slope_item_get_metrics (const slope_item_t *item)
{
    if (item == NULL) {
        return NULL;
    }
    return item->metrics;
}


slope_figure_t* slope_item_get_figure (const slope_item_t *item)
{
    if (item == NULL) {
        return NULL;
    }
    return slope_metrics_get_figure(item->metrics);
}


void slope_item_notify_appearence_change (slope_item_t *item)
{
    slope_figure_t *figure = slope_item_get_figure(item);
    slope_figure_notify_appearence_change(figure, item);
}


void slope_item_notify_data_change (slope_item_t *item)
{
    slope_figure_t *figure = slope_item_get_figure(item);
    slope_figure_notify_data_change(figure, item);
}


int __slope_item_parse_color (const char *fmt)
{
    while (*fmt) {
        if (*fmt == 'b') return SLOPE_BLACK;
        if (*fmt == 'w') return SLOPE_WHITE;
        if (*fmt == 'r') return SLOPE_RED;
        if (*fmt == 'g') return SLOPE_GREEN;
        if (*fmt == 'l') return SLOPE_BLUE;
        ++fmt;
    }
    return SLOPE_BLACK;
}


int __slope_item_parse_scatter (const char *fmt)
{
    while (*fmt) {
        if (*fmt == '-') return SLOPE_LINE;
        if (*fmt == '*') return SLOPE_CIRCLES;
        if (*fmt == '+') return SLOPE_PLUSSES;
        ++fmt;
    }
    return SLOPE_LINE;
}

/* slope/item.c */
