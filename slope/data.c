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

#include "slope/data_p.h"
#include "slope/metrics.h"
#include "slope/scene.h"
#include <stdlib.h>
#include <string.h>


void slope_data_destroy (slope_data_t *data)
{
    if (data == NULL) {
        return;
    }
    if (data->klass->destroy_fn) {
        (*data->klass->destroy_fn)(data);
    }
    if (data->name) {
        free(data->name);
    }
    free(data);
}


int slope_data_get_visible (slope_data_t *data)
{
    if (data == NULL) {
        return SLOPE_FALSE;
    }
    return data->visible;
}


void slope_data_set_visible (slope_data_t *data,
                             int visible)
{
    if (data == NULL) {
        return;
    }
    data->visible = visible;
    slope_data_notify_appearence_change(data);
}


const char* slope_data_get_name (slope_data_t *data)
{
    if (data == NULL) {
        return NULL;
    }
    return data->name;
}


void slope_data_set_name (slope_data_t *data, const char *name)
{
    if (data == NULL) {
        return;
    }
    if (data->name) {
        free(data->name);
    }
    data->name = strdup(name);
    slope_data_notify_appearence_change(data);
}


void __slope_data_draw (slope_data_t *data, cairo_t *cr,
                        const slope_metrics_t *metrics)
{
    (*data->klass->draw_fn)(data, cr, metrics);
}


slope_metrics_t* slope_data_get_metrics (slope_data_t *data)
{
    if (data == NULL) {
        return NULL;
    }
    return data->metrics;
}


slope_scene_t* slope_data_get_scene (slope_data_t *data)
{
    if (data == NULL) {
        return NULL;
    }
    return slope_metrics_get_scene(data->metrics);
}


void slope_data_notify_appearence_change (slope_data_t *data)
{
    slope_scene_t *scene = slope_data_get_scene(data);
    slope_scene_notify_appearence_change(scene, data);
}


void slope_data_notify_data_change (slope_data_t *data)
{
    slope_scene_t *scene = slope_data_get_scene(data);
    slope_scene_notify_data_change(scene, data);
}


int __slope_data_parse_color (const char *fmt)
{
    static int undefine_color=SLOPE_WHITE;
    while (*fmt) {
        if (*fmt == 'b') return SLOPE_BLACK;
        if (*fmt == 'w') return SLOPE_WHITE;
        if (*fmt == 'r') return SLOPE_RED;
        if (*fmt == 'g') return SLOPE_GREEN;
        if (*fmt == 'l') return SLOPE_BLUE;
        if (*fmt == 'y') return SLOPE_YELLOW;
        if (*fmt == 'm') return SLOPE_MAROON;
        if (*fmt == 'e') return SLOPE_GREY;
        if (*fmt == 'p') return SLOPE_PURPLE;
        if (*fmt == 'i') return SLOPE_OLIVE;
        if (*fmt == 't') return SLOPE_TEAL;
        if (*fmt == 'o') return SLOPE_ORANGE;
        if (*fmt == 'u')
        {
            ++undefine_color;
            if (undefine_color == SLOPE_LAST_COLOR)
                undefine_color=SLOPE_RED;
            return undefine_color;
        }
        ++fmt;
    }
    return SLOPE_BLACK;
}


int __slope_data_parse_scatter (const char *fmt)
{
    while (*fmt) {
        if (*fmt == '-') return SLOPE_SQUARES;
        if (*fmt == '.') return SLOPE_CIRCLES;
        if (*fmt == '+') return SLOPE_PLUSSES;
        if (*fmt == '/') return SLOPE_TRIANGLES;
        ++fmt;
    }
    return -1;
}

/* slope/data.c */

