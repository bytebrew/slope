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

#include <slope/legend_p.h>
#include <slope/list.h>
#include <slope/scale.h>
#include <slope/figure.h>
#include <stdlib.h>


static void _slope_legend_get_data_rect (const slope_item_t *self, slope_rect_t *rect);
static void _slope_legend_get_figure_rect (const slope_item_t *self, slope_rect_t *rect);
static void _slope_legend_draw (slope_item_t *self, cairo_t *cr);
static void _slope_legend_eval_rect (slope_item_t *self, cairo_t *cr);


static slope_item_class_t* _slope_legend_get_class()
{
    static slope_item_class_t item_class;
    static slope_bool_t first_call = SLOPE_TRUE;
    
    if (first_call) {
        item_class.init = slope_legend_init;
        item_class.finalize = slope_legend_finalize;
        item_class.draw = _slope_legend_draw;
        item_class.get_data_rect = _slope_legend_get_data_rect;
        item_class.get_figure_rect = _slope_legend_get_figure_rect;
        first_call = SLOPE_FALSE;
    }
    return &item_class;
}


slope_item_t* slope_legend_new (slope_figure_t *figure)
{
    slope_legend_t *self = SLOPE_ALLOC(slope_legend_t);
    slope_legend_private_t *priv = SLOPE_ALLOC(slope_legend_private_t);
    
    SLOPE_ITEM(self)->_private = SLOPE_ITEM_PRIVATE(priv);
    SLOPE_ITEM(self)->_class = _slope_legend_get_class();
    SLOPE_ITEM_GET_CLASS(self)->init(SLOPE_ITEM(self));
    
    priv->figure = figure;
    return SLOPE_ITEM(self);
}


void slope_legend_init (slope_item_t *self)
{
    slope_legend_private_t *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    slope_item_init(self);
    
    priv->position_policy = SLOPE_LEGEND_TOPRIGHT;
    priv->fill_color = SLOPE_WHITE;
    priv->stroke_color = SLOPE_BLACK;
}


void slope_legend_finalize (slope_item_t *self)
{
    slope_item_finalize(self);
}


static void _slope_legend_draw (slope_item_t *self, cairo_t *cr)
{
    slope_legend_private_t *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    slope_list_t *scale_list, *item_list;
    slope_iterator_t *scale_iter, *item_iter;
    double x, y;
    
    _slope_legend_eval_rect (self, cr);
    if (priv->line_count == 0)
        return;
    
    slope_cairo_rect(cr, &priv->rect);
    slope_cairo_set_color(cr, priv->fill_color);
    cairo_fill_preserve(cr);
    slope_cairo_set_color(cr, priv->stroke_color);
    cairo_stroke(cr);
    
    y = priv->rect.y + priv->line_height + 5.0;
    x = priv->rect.x + 40.0;
    scale_list = slope_figure_get_scale_list(priv->figure);
    SLOPE_LIST_FOREACH(scale_iter, scale_list) {
        slope_scale_t *curr_scale = SLOPE_SCALE(slope_iterator_data(scale_iter));
        item_list = slope_scale_get_item_list(curr_scale);
        SLOPE_LIST_FOREACH(item_iter, item_list) {
            slope_item_t *curr_item = SLOPE_ITEM(slope_iterator_data(item_iter));
            
            if (slope_item_get_visible(curr_item) == SLOPE_FALSE)
                continue;

            cairo_move_to(cr, x, y);
            cairo_show_text(cr, slope_item_get_name(curr_item));
            y += priv->line_height;
        }
    }
}


static void _slope_legend_get_data_rect (const slope_item_t *self, slope_rect_t *rect)
{
    
}


static void _slope_legend_get_figure_rect (const slope_item_t *self, slope_rect_t *rect)
{
    /* TODO */
}


static void _slope_legend_eval_rect (slope_item_t *self, cairo_t *cr)
{
    slope_legend_private_t *priv = SLOPE_LEGEND_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    slope_rect_t fig_rect;
    cairo_text_extents_t txt_ext;
    slope_list_t *scale_list, *item_list;
    slope_iterator_t *scale_iter, *item_iter;
    slope_figure_t *figure;
    double txt_hei, max_wid = 0.0;

    slope_scale_get_figure_rect(item_priv->scale, &fig_rect);
    
    /* work around crazy heights returned when no ascii symbols are present */
    cairo_text_extents(cr, "dummy", &txt_ext);
    txt_hei = 0.66 * txt_ext.height;
    
    scale_list = slope_figure_get_scale_list(priv->figure);
    priv->line_count = 0;
    SLOPE_LIST_FOREACH(scale_iter, scale_list) {
        slope_scale_t *curr_scale = SLOPE_SCALE(slope_iterator_data(scale_iter));
        item_list = slope_scale_get_item_list(curr_scale);
        SLOPE_LIST_FOREACH(item_iter, item_list) {
            slope_item_t *curr_item = SLOPE_ITEM(slope_iterator_data(item_iter));
            
            if (slope_item_get_visible(curr_item) == SLOPE_FALSE)
                continue;
            priv->line_count += 1;
            
            cairo_text_extents(cr, slope_item_get_name(curr_item), &txt_ext);
            if (txt_ext.width > max_wid) max_wid = txt_ext.width;
        }
    }
    
    if (priv->line_count == 0)
        return;
    priv->rect.width = max_wid + 50.0;
    priv->rect.height = 1.5 * txt_hei * priv->line_count + 15.0;
    priv->line_height = 1.5 * txt_hei;

    switch (priv->position_policy) {
        case SLOPE_LEGEND_TOPRIGHT:
            priv->rect.x = fig_rect.x + fig_rect.width - priv->rect.width - 12.0;
            priv->rect.y = fig_rect.y + 12.0;
            break;
    }
}

/* slope/legend.c */
