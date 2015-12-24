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

#include <slope/linear_p.h>
#include <slope/item_p.h>
#include <stdlib.h>
#include <stdio.h>


static void _slope_linear_draw (slope_scale_t *self, const slope_rect_t *rect, cairo_t *cr);
static void _slope_linear_map (slope_scale_t *self, slope_point_t *figure_point, const slope_point_t *data_point);
static void _slope_linear_get_data_rect (const slope_scale_t *self, slope_rect_t *rect);
static void _slope_linear_get_figure_rect (const slope_scale_t *self, slope_rect_t *rect);
static void _slope_linear_update (slope_scale_t *self);
static void _slope_linear_update_for_item (slope_scale_t *self, slope_item_t *item);
static void _slope_linear_set_color_scheme (slope_scale_t*, slope_color_t, slope_color_t, slope_color_t);


static slope_scale_class_t* _slope_linear_get_class ()
{
    static slope_scale_class_t scale_class;
    static slope_bool_t first_call = SLOPE_TRUE;

    if (first_call) {
        scale_class.init = slope_linear_init;
        scale_class.finalize = slope_linear_finalize;
        scale_class.draw = _slope_linear_draw;
        scale_class.map = _slope_linear_map;
        scale_class.get_data_rect = _slope_linear_get_data_rect;
        scale_class.get_figure_rect = _slope_linear_get_figure_rect;
        scale_class.update = _slope_linear_update;
        scale_class.update_for_item = _slope_linear_update_for_item;
        scale_class.set_color_scheme = _slope_linear_set_color_scheme;
        first_call = SLOPE_FALSE;
    }

    return &scale_class;
}


slope_scale_t* slope_linear_new (const char *name)
{
    slope_linear_t *self = SLOPE_ALLOC(slope_linear_t);
    slope_linear_private_t *priv = SLOPE_ALLOC(slope_linear_private_t);
    slope_item_t *axis;
    
    SLOPE_SCALE(self)->_private = SLOPE_SCALE_PRIVATE(priv);
    SLOPE_SCALE(self)->_class = _slope_linear_get_class();
    SLOPE_SCALE_GET_CLASS(self)->init(SLOPE_SCALE(self));

    /* add axis */
    axis = slope_axis_new(SLOPE_SCALE(self), "X", SLOPE_AXIS_BOTTOM);
    slope_list_append(priv->axis_list, axis);
    axis = slope_axis_new(SLOPE_SCALE(self), "X", SLOPE_AXIS_TOP);
    slope_axis_set_elements(axis, SLOPE_AXIS_LINE);
    slope_list_append(priv->axis_list, axis);
    axis = slope_axis_new(SLOPE_SCALE(self), "Y", SLOPE_AXIS_LEFT);
    slope_list_append(priv->axis_list, axis);
    axis = slope_axis_new(SLOPE_SCALE(self), "Y", SLOPE_AXIS_RIGHT);
    slope_axis_set_elements(axis, SLOPE_AXIS_LINE);
    slope_list_append(priv->axis_list, axis);

    slope_scale_update(SLOPE_SCALE(self));
    return SLOPE_SCALE(self);
}


void slope_linear_init (slope_scale_t *self)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);

    slope_scale_init(self);

    priv->axis_list = slope_list_new();
    priv->x_low_bound = 80.0; priv->x_up_bound = 20.0;
    priv->y_low_bound = 30.0; priv->y_up_bound = 45.0;
}


void slope_linear_finalize (slope_scale_t *self)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    slope_iterator_t *iter;

    SLOPE_LIST_FOREACH (iter, priv->axis_list) {
        slope_item_t *axis = SLOPE_ITEM(slope_iterator_data(iter));
        slope_item_destroy(axis);
    }
    slope_list_destroy(priv->axis_list);

    slope_scale_finalize(self);
}


static void _slope_linear_draw (slope_scale_t *self,
                                const slope_rect_t *rect, cairo_t *cr)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    slope_scale_private_t *scale_priv = SLOPE_SCALE_PRIVATE(priv);
    slope_iterator_t *iter;
    
    priv->fig_x_min = rect->x + priv->x_low_bound;
    priv->fig_x_max = rect->x + rect->width - priv->x_up_bound;
    priv->fig_y_min = rect->y + priv->y_low_bound;
    priv->fig_y_max = rect->y + rect->height - priv->y_up_bound;
    priv->fig_width = priv->fig_x_max - priv->fig_x_min;
    priv->fig_height = priv->fig_y_max - priv->fig_y_min;

    cairo_save(cr);
    cairo_new_path(cr);

    cairo_rectangle(cr, priv->fig_x_min, priv->fig_y_min,
                    priv->fig_width, priv->fig_height);
    cairo_clip(cr);
    
    SLOPE_LIST_FOREACH (iter, scale_priv->item_list) {
        slope_item_t *item = SLOPE_ITEM(slope_iterator_data(iter));
        if (slope_item_get_visible(item)) {
            _slope_item_draw(item, cr);
        }
    }

    cairo_restore(cr);

    SLOPE_LIST_FOREACH (iter, priv->axis_list) {
        slope_item_t *axis = SLOPE_ITEM(slope_iterator_data(iter));
        if (slope_item_get_visible(axis)) {
            _slope_item_draw(axis, cr);
        }
    }
}


static void _slope_linear_map (slope_scale_t *self, slope_point_t *figure_point,
                               const slope_point_t *data_point)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    double tmp;
    
    tmp = (data_point->x - priv->dat_x_min) / priv->dat_width;
    figure_point->x = priv->fig_x_min + tmp*priv->fig_width;
    
    tmp = (data_point->y - priv->dat_y_min) / priv->dat_height;
    figure_point->y = priv->fig_y_max - tmp*priv->fig_height;
}


static void _slope_linear_get_data_rect (const slope_scale_t *self, slope_rect_t *rect)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);

    rect->x = priv->dat_x_min;
    rect->y = priv->dat_y_min;
    rect->width = priv->dat_width;
    rect->height = priv->dat_height;
}


static void _slope_linear_get_figure_rect (const slope_scale_t *self, slope_rect_t *rect)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);

    rect->x = priv->fig_x_min;
    rect->y = priv->fig_y_min;
    rect->width = priv->fig_width;
    rect->height = priv->fig_height;
}


static void _slope_linear_update (slope_scale_t *self)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    slope_scale_private_t *scale_priv = SLOPE_SCALE_PRIVATE(priv);
    slope_iterator_t *item_iter;
    slope_item_t *item;
    slope_rect_t item_rect;
    double bound;
    
    if (slope_list_size(scale_priv->item_list) == 0) {
        priv->dat_x_min = 0.0;
        priv->dat_x_max = 1.0;
        priv->dat_y_min = 0.0;
        priv->dat_y_max = 1.0;
        priv->dat_width = 1.0;
        priv->dat_height = 1.0;
        return;
    }
    
    item_iter = slope_list_first(scale_priv->item_list);
    item = SLOPE_ITEM(slope_iterator_data(item_iter));
    slope_item_get_data_rect(item, &item_rect);
    
    priv->dat_x_min = item_rect.x;
    priv->dat_x_max = item_rect.x + item_rect.width;
    priv->dat_y_min = item_rect.y;
    priv->dat_y_max = item_rect.y + item_rect.height;
    priv->dat_width = item_rect.width;
    priv->dat_height = item_rect.height;
    
    SLOPE_LIST_FOREACH (item_iter, scale_priv->item_list) {
        double xmin, xmax;
        double ymin, ymax;

        item = SLOPE_ITEM(slope_iterator_data(item_iter));
        slope_item_get_data_rect(item, &item_rect);

        xmin = item_rect.x;
        xmax = item_rect.x + item_rect.width;
        ymin = item_rect.y;
        ymax = item_rect.y + item_rect.height;

        if (xmin < priv->dat_x_min) priv->dat_x_min = xmin;
        if (xmax > priv->dat_x_max) priv->dat_x_max = xmax;
        if (ymin < priv->dat_y_min) priv->dat_y_min = ymin;
        if (ymax > priv->dat_y_max) priv->dat_y_max = ymax;
    }

    priv->dat_width = priv->dat_x_max - priv->dat_x_min;
    bound = priv->dat_width / 40.0;
    priv->dat_x_min -= bound;
    priv->dat_x_max += bound;
    priv->dat_width += 2.0*bound;
    
    priv->dat_height = priv->dat_y_max - priv->dat_y_min;
    bound = priv->dat_height / 40.0;
    priv->dat_y_min -= bound;
    priv->dat_y_max += bound;
    priv->dat_height += 2.0*bound;
}


static void _slope_linear_update_for_item (slope_scale_t *self, slope_item_t *item)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    slope_scale_private_t *scale_priv = SLOPE_SCALE_PRIVATE(priv);
    slope_rect_t item_rect;
    double xmin, xmax;
    double ymin, ymax;
    double bound;
    
    if (!slope_list_contains(scale_priv->item_list, item))
        return;

    slope_item_get_data_rect(item, &item_rect);

    xmin = item_rect.x;
    xmax = item_rect.x + item_rect.width;
    ymin = item_rect.y;
    ymax = item_rect.y + item_rect.height;

    if (slope_list_size(scale_priv->item_list) == 1) {
        priv->dat_x_min = xmin;
        priv->dat_x_max = xmax;
        priv->dat_y_min = ymin;
        priv->dat_y_max = ymax;
    }
    else {
        if (xmin < priv->dat_x_min) priv->dat_x_min = xmin;
        if (xmax > priv->dat_x_max) priv->dat_x_max = xmax;
        if (ymin < priv->dat_y_min) priv->dat_y_min = ymin;
        if (ymax > priv->dat_y_max) priv->dat_y_max = ymax;
    }
    
    priv->dat_width = priv->dat_x_max - priv->dat_x_min;
    bound = priv->dat_width / 40.0;
    priv->dat_x_min -= bound;
    priv->dat_x_max += bound;
    priv->dat_width += 2.0*bound;
    
    priv->dat_height = priv->dat_y_max - priv->dat_y_min;
    bound = priv->dat_height / 40.0;
    priv->dat_y_min -= bound;
    priv->dat_y_max += bound;
    priv->dat_height += 2.0*bound;
}


slope_item_t* slope_linear_get_axis (slope_scale_t *self, slope_axis_position_t pos)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    slope_iterator_t *iter;

    SLOPE_LIST_FOREACH (iter, priv->axis_list) {
       slope_item_t *axis = SLOPE_ITEM(slope_iterator_data(iter));
       if (slope_axis_get_position(axis) == pos)
          return axis;
    }
    return NULL;
}


void slope_linear_set_show_grid (slope_scale_t *self, slope_bool_t show)
{
    slope_item_t *axis;
    int elements = show;
    
    axis = slope_linear_get_axis (self, SLOPE_AXIS_BOTTOM);
    elements = show ? slope_axis_get_elements(axis) | SLOPE_AXIS_GRID :
                      slope_axis_get_elements(axis) & ~SLOPE_AXIS_GRID;
    slope_axis_set_elements(axis, elements);
        
    axis = slope_linear_get_axis (self, SLOPE_AXIS_LEFT);
    elements = show ? slope_axis_get_elements(axis) | SLOPE_AXIS_GRID :
                      slope_axis_get_elements(axis) & ~SLOPE_AXIS_GRID;
    slope_axis_set_elements(axis, elements);
    
    axis = slope_linear_get_axis (self, SLOPE_AXIS_TOP);
    elements = slope_axis_get_elements(axis) & ~SLOPE_AXIS_GRID;
    slope_axis_set_elements(axis, elements);
    
    axis = slope_linear_get_axis (self, SLOPE_AXIS_RIGHT);
    elements = slope_axis_get_elements(axis) & ~SLOPE_AXIS_GRID;
    slope_axis_set_elements(axis, elements);
}


void slope_linear_set_x_boundaries(slope_scale_t *self, double low_bound, double up_bound)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    priv->x_low_bound = low_bound;
    priv->x_up_bound = up_bound;
}


void slope_linear_set_y_boundaries(slope_scale_t *self, double low_bound, double up_bound)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    priv->y_low_bound = low_bound;
    priv->y_up_bound = up_bound;
}


void slope_linear_set_axis_labels (slope_scale_t *self,
                                   const char *bottom_label, const char *left_label,
                                   const char *top_label, const char *right_label)
{
    slope_item_set_name (slope_linear_get_axis (self, SLOPE_AXIS_BOTTOM), bottom_label);
    slope_item_set_name (slope_linear_get_axis (self, SLOPE_AXIS_LEFT),   left_label);
    slope_item_set_name (slope_linear_get_axis (self, SLOPE_AXIS_TOP),    top_label);
    slope_item_set_name (slope_linear_get_axis (self, SLOPE_AXIS_RIGHT),  right_label);
}


void _slope_linear_set_color_scheme (slope_scale_t *self, slope_color_t background,
                                     slope_color_t foreground, slope_color_t extra_color)
{
    slope_linear_private_t *priv = SLOPE_LINEAR_GET_PRIVATE(self);
    slope_iterator_t *iter;
    
    SLOPE_LIST_FOREACH (iter, priv->axis_list)
        slope_axis_set_colors (SLOPE_ITEM(slope_iterator_data(iter)),
                               foreground, foreground);
}

/* slope/linear.c */
