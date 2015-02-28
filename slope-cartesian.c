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

#include "slope-cartesian_p.h"
#include "slope-scatter_p.h"
#include <stdlib.h>

slope_plotable_t* slope_cartesian_create ()
{
    slope_cartesian_t *cart = malloc(sizeof(slope_cartesian_t));
    slope_plotable_t *base = (slope_plotable_t*) cart;
    base->visib = 1;
    cart->x_low_b = cart->x_up_b = 20.0;
    cart->y_low_b = cart->y_up_b = 20.0;
    base->_cleanup_fn = _slope_cartesian_cleanup;
    base->_draw_fn = _slope_cartesian_draw;
    cart->scatters = NULL;
    slope_cartesian_rescale(base);
    return base;
}

void _slope_cartesian_cleanup (slope_plotable_t *base)
{
    slope_cartesian_t *cartesian = (slope_cartesian_t*) base;
    slope_list_destroy(cartesian->scatters);
}

void _slope_cartesian_draw (slope_plotable_t *base,
                            cairo_t *cr, slope_rect_t *scene_rect)
{
    slope_cartesian_t *cart = (slope_cartesian_t*) base;
    _slope_cartesian_set_scene_rect(base, scene_rect);
    
    cairo_save(cr);
    cairo_rectangle(cr, cart->x_low_b+1.0, cart->y_low_b+1.0,
                    cart->width_scene, cart->height_scene);
    cairo_clip(cr);
    slope_iterator_t *iter = slope_list_first(cart->scatters);
    while (iter) {
        slope_scatter_t *scat =
            (slope_scatter_t*) slope_iterator_data(iter);
        if (slope_scatter_visible(scat)) {
            _slope_scatter_draw(scat, base, cr);
        }
        slope_iterator_next(&iter);
    }
    cairo_restore(cr);
}

double slope_cartesian_map_x (slope_plotable_t *base, double x)
{
    slope_cartesian_t *cart = (slope_cartesian_t*) base;
    double ret = (x - cart->x_min) /cart->width;
    return cart->x_min_scene + ret*cart->width_scene;
}

double slope_cartesian_map_y (slope_plotable_t *base, double y)
{
    slope_cartesian_t *cart = (slope_cartesian_t*) base;
    double ret = (y - cart->y_min) /cart->height;
    return cart->y_max_scene - ret*cart->height_scene;
}

void slope_cartesian_add_scatter (slope_plotable_t *base,
                                  slope_scatter_t *scatter)
{
    slope_cartesian_t *cart = (slope_cartesian_t*) base;
    cart->scatters = slope_list_append(cart->scatters, scatter);
    slope_cartesian_rescale(base);
}

void slope_cartesian_rescale (slope_plotable_t *base)
{
    slope_cartesian_t *cart = (slope_cartesian_t*) base;
    if (cart->scatters == NULL) {
        cart->x_min = 0.0;
        cart->x_max = 1.0;
        cart->y_min = 0.0;
        cart->y_max = 1.0;
        return;
    }
    slope_iterator_t *iter = slope_list_first(cart->scatters);
    slope_scatter_t *scat = (slope_scatter_t*) slope_iterator_data(iter);
    cart->x_min = scat->x_min;
    cart->x_max = scat->x_max;
    cart->y_min = scat->y_min;
    cart->y_max = scat->y_max;
    slope_iterator_next(&iter);
    while (iter) {
        scat = (slope_scatter_t*) slope_iterator_data(iter);
        if (scat->x_min < cart->x_min) cart->x_min = scat->x_min;
        if (scat->x_max > cart->x_max) cart->x_max = scat->x_max;
        if (scat->y_min < cart->y_min) cart->y_min = scat->y_min;
        if (scat->y_max > cart->y_max) cart->y_max = scat->y_max;
        slope_iterator_next(&iter);
    }
    cart->width = cart->x_max - cart->x_min;
    cart->height = cart->y_max - cart->y_min;
}

void _slope_cartesian_set_scene_rect (slope_plotable_t* base,
                                      slope_rect_t *scene_rect)
{
    slope_cartesian_t *cart = (slope_cartesian_t*) base;
    cart->x_min_scene = scene_rect->x + cart->x_low_b;
    cart->y_min_scene = scene_rect->y + cart->y_low_b;
    cart->x_max_scene = scene_rect->x + scene_rect->width - cart->x_up_b;
    cart->y_max_scene = scene_rect->y + scene_rect->height - cart->y_up_b;
    cart->width_scene = cart->x_max_scene - cart->x_min_scene;
    cart->height_scene = cart->y_max_scene - cart->y_min_scene;
}

/* slope-cartesian.c */