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

#include <slope/axis_p.h>
#include <stdlib.h>
#include <stdio.h>


static const double dashes[2] = { 4.0, 4.0 };


static void _slope_axis_get_data_rect (const slope_item_t *self, slope_rect_t *rect);
static void _slope_axis_get_figure_rect (const slope_item_t *self, slope_rect_t *rect);
static void _slope_axis_draw (slope_item_t *self, cairo_t *cr);
static void _slope_axis_draw_bottom (slope_item_t *self, cairo_t *cr);
static void _slope_axis_draw_top (slope_item_t *self, cairo_t *cr);
static void _slope_axis_draw_left (slope_item_t *self, cairo_t *cr);
static void _slope_axis_draw_right (slope_item_t *self, cairo_t *cr);


static slope_item_class_t* _slope_axis_get_class()
{
    static slope_item_class_t item_class;
    static slope_bool_t first_call = SLOPE_TRUE;
    
    if (first_call) {
        item_class.init = slope_axis_init;
        item_class.finalize = slope_axis_finalize;
        item_class.draw = _slope_axis_draw;
        item_class.draw_thumb = _slope_item_draw_thumb_dummy_impl;
        item_class.get_data_rect = _slope_axis_get_data_rect;
        item_class.get_figure_rect = _slope_axis_get_figure_rect;
        first_call = SLOPE_FALSE;
    }
    return &item_class;
}


slope_item_t* slope_axis_new (slope_scale_t *linear_scale, const char *name, slope_axis_position_t pos)
{
    slope_axis_t *self = SLOPE_ALLOC(slope_axis_t);
    slope_axis_private_t *priv = SLOPE_ALLOC(slope_axis_private_t);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    
    SLOPE_ITEM(self)->_private = SLOPE_ITEM_PRIVATE(priv);
    SLOPE_ITEM(self)->_class = _slope_axis_get_class();
    SLOPE_ITEM_GET_CLASS(self)->init(SLOPE_ITEM(self));
    
    priv->pos = pos;
    priv->color = SLOPE_BLACK;
    priv->grid_color = SLOPE_LIGHTGRAY;
    item_priv->scale = linear_scale;
    priv->elements = SLOPE_AXIS_ALL;
    priv->elements &= ~SLOPE_AXIS_GRID;
    slope_item_set_name(SLOPE_ITEM(self), name);

    return SLOPE_ITEM(self);
}


void slope_axis_init (slope_item_t *self)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_init(self);

    priv->sampler = slope_sampler_new();
}


void slope_axis_finalize (slope_item_t *self)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);

    slope_sampler_destroy(priv->sampler);
    slope_item_finalize(self);
}


static void _slope_axis_draw (slope_item_t *self, cairo_t *cr)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);

    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width(cr, 1.0);
    slope_cairo_set_color(cr, priv->color);

    switch (priv->pos) {
        case SLOPE_AXIS_BOTTOM:
            _slope_axis_draw_bottom(self, cr);
            break;
        case SLOPE_AXIS_TOP:
            _slope_axis_draw_top(self, cr);
            break;
        case SLOPE_AXIS_LEFT:
            _slope_axis_draw_left(self, cr);
            break;
        case SLOPE_AXIS_RIGHT:
            _slope_axis_draw_right(self, cr);
            break;
    }
}


static void _slope_axis_get_data_rect (const slope_item_t *self, slope_rect_t *rect)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    slope_scale_get_data_rect(item_priv->scale, rect);
    
    switch (priv->pos) {
        case SLOPE_AXIS_TOP:
            rect->height = 0.0;
            break;
        case SLOPE_AXIS_BOTTOM:
            rect->y += rect->height;
            rect->height = 0.0;
            break;
        case SLOPE_AXIS_LEFT:
            rect->width = 0.0;
            break;
        case SLOPE_AXIS_RIGHT:
            rect->x += rect->width;
            rect->width = 0.0;
            break;
    }
}


static void _slope_axis_get_figure_rect (const slope_item_t *self, slope_rect_t *rect)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    slope_scale_get_figure_rect(item_priv->scale, rect);

    switch (priv->pos) {
        case SLOPE_AXIS_TOP:
            rect->height = 0.0;
            break;
        case SLOPE_AXIS_BOTTOM:
            rect->y += rect->height;
            rect->height = 0.0;
            break;
        case SLOPE_AXIS_LEFT:
            rect->width = 0.0;
            break;
        case SLOPE_AXIS_RIGHT:
            rect->x += rect->width;
            rect->width = 0.0;
            break;
    }
}


static void _slope_axis_draw_bottom (slope_item_t *self, cairo_t *cr)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    cairo_text_extents_t txt_ext;
    slope_list_t *list;
    slope_iterator_t *iter;
    slope_rect_t fig_rect;
    slope_rect_t dat_rect;
    double x, y, txt_hei;

    /* work around crazy text heights when using unicode */
    cairo_text_extents(cr, "dummy", &txt_ext);
    txt_hei = txt_ext.height * 0.66;

    slope_scale_get_figure_rect(item_priv->scale, &fig_rect);
    slope_scale_get_data_rect(item_priv->scale, &dat_rect);
    x = fig_rect.x;
    y = fig_rect.y + fig_rect.height;
    
    if (priv->elements & SLOPE_AXIS_LINE) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x+fig_rect.width, y);
        cairo_stroke(cr);
    }

    if ((priv->elements & SLOPE_AXIS_TICKS) && dat_rect.width > 0.0) {
        if (slope_sampler_get_mode(priv->sampler) == SLOPE_SAMPLER_AUTO_DECIMAL)
            slope_sampler_auto_sample_decimal(priv->sampler, dat_rect.x, dat_rect.x+dat_rect.width,
                                              fig_rect.width/80.0);
        list = slope_sampler_get_sample_list(priv->sampler);

        SLOPE_LIST_FOREACH (iter, list) {
            slope_sample_t *sample = SLOPE_SAMPLE(slope_iterator_data(iter));
            slope_point_t fig_pt;
            slope_point_t dat_pt;

            if (sample->value < dat_rect.x ||
                sample->value > dat_rect.x + dat_rect.width) {
                continue;
            }

            dat_pt.x = sample->value;
            dat_pt.y = 42.0;
            slope_scale_map(item_priv->scale, &fig_pt, &dat_pt);
            x = fig_pt.x;

            if ((priv->elements & SLOPE_AXIS_GRID) && sample->is_major) {
                cairo_save(cr);
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y-fig_rect.height);
                cairo_set_dash(cr, dashes, 2, 0.0);
                slope_cairo_set_color(cr, priv->grid_color);
                cairo_stroke(cr);
                cairo_restore(cr);
            }

            if (sample->is_major) {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y-6.0);
                cairo_stroke(cr);

                cairo_text_extents(cr, sample->label, &txt_ext);
                cairo_move_to(cr, x-txt_ext.width/2.0, y+2.0*txt_hei);
                cairo_show_text(cr, sample->label);
            }
            else {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y-3.0);
                cairo_stroke(cr);
            }
        }
    }
    if (priv->elements & SLOPE_AXIS_TITLE) {
        cairo_text_extents(cr, item_priv->name, &txt_ext);
        x = fig_rect.x + (fig_rect.width - txt_ext.width)/2.0;
        y += 4.0*txt_hei;
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, item_priv->name);
    }
}


static void _slope_axis_draw_top (slope_item_t *self, cairo_t *cr)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    cairo_text_extents_t txt_ext;
    slope_list_t *list;
    slope_iterator_t *iter;
    slope_rect_t fig_rect;
    slope_rect_t dat_rect;
    double x, y, txt_hei;

    /* work around crazy text heights when using unicode */
    cairo_text_extents(cr, "dummy", &txt_ext);
    txt_hei = txt_ext.height * 0.66;

    slope_scale_get_figure_rect(item_priv->scale, &fig_rect);
    slope_scale_get_data_rect(item_priv->scale, &dat_rect);
    fig_rect.x -= 0.5;
    fig_rect.width += 1.0;
    x = fig_rect.x;
    y = fig_rect.y;
    
    if (priv->elements & SLOPE_AXIS_LINE) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x+fig_rect.width, y);
        cairo_stroke(cr);
    }

    if ((priv->elements & SLOPE_AXIS_TICKS) && dat_rect.width > 0.0) {
        if (slope_sampler_get_mode(priv->sampler) == SLOPE_SAMPLER_AUTO_DECIMAL)
            slope_sampler_auto_sample_decimal(priv->sampler, dat_rect.x, dat_rect.x+dat_rect.width,
                                              fig_rect.width/80.0);
        list = slope_sampler_get_sample_list(priv->sampler);

        SLOPE_LIST_FOREACH (iter, list) {
            slope_sample_t *sample = SLOPE_SAMPLE(slope_iterator_data(iter));
            slope_point_t fig_pt;
            slope_point_t dat_pt;

            if (sample->value < dat_rect.x ||
                sample->value > dat_rect.x + dat_rect.width) {
                continue;
            }

            dat_pt.x = sample->value;
            dat_pt.y = 42.0;
            slope_scale_map(item_priv->scale, &fig_pt, &dat_pt);
            x = fig_pt.x;
            
            if (sample->is_major) {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y+6.0);
                cairo_stroke(cr);

                cairo_text_extents(cr, sample->label, &txt_ext);
                cairo_move_to(cr, x-txt_ext.width/2.0, y-txt_hei);
                cairo_show_text(cr, sample->label);
            }
            else {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y+3.0);
                cairo_stroke(cr);
            }
        }
    }
    if (priv->elements & SLOPE_AXIS_TITLE) {
        cairo_text_extents(cr, item_priv->name, &txt_ext);
        x = fig_rect.x + (fig_rect.width - txt_ext.width)/2.0;
        y -= 3.0*txt_hei;
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, item_priv->name);
    }
}


static void _slope_axis_draw_left (slope_item_t *self, cairo_t *cr)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    cairo_text_extents_t txt_ext;
    slope_list_t *list;
    slope_iterator_t *iter;
    slope_rect_t fig_rect;
    slope_rect_t dat_rect;
    double x, y, txt_hei;
    double txt_max_width;

    /* work around crazy text heights when using unicode */
    cairo_text_extents(cr, "dummy", &txt_ext);
    txt_hei = txt_ext.height * 0.66;
    txt_max_width = 0.0;

    slope_scale_get_figure_rect(item_priv->scale, &fig_rect);
    slope_scale_get_data_rect(item_priv->scale, &dat_rect);
    fig_rect.height += 0.5;
    x = fig_rect.x;
    y = fig_rect.y + fig_rect.height;

    if (priv->elements & SLOPE_AXIS_LINE) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y-fig_rect.height);
        cairo_stroke(cr);
    }

    if ((priv->elements & SLOPE_AXIS_TICKS) && dat_rect.height > 0.0) {
        if (slope_sampler_get_mode(priv->sampler) == SLOPE_SAMPLER_AUTO_DECIMAL)
            slope_sampler_auto_sample_decimal(priv->sampler, dat_rect.y, dat_rect.y+dat_rect.height,
                                              fig_rect.height/60.0);
        list = slope_sampler_get_sample_list(priv->sampler);

        SLOPE_LIST_FOREACH (iter, list) {
            slope_sample_t *sample = SLOPE_SAMPLE(slope_iterator_data(iter));
            slope_point_t fig_pt;
            slope_point_t dat_pt;

            if (sample->value < dat_rect.y ||
                sample->value > dat_rect.y + dat_rect.height) {
                continue;
            }

            dat_pt.x = 42.0;
            dat_pt.y = sample->value;
            slope_scale_map(item_priv->scale, &fig_pt, &dat_pt);
            y = fig_pt.y;

            if ((priv->elements & SLOPE_AXIS_GRID) && sample->is_major) {
                cairo_save(cr);
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x+fig_rect.width, y);
                slope_cairo_set_color(cr, priv->grid_color);
                cairo_set_dash(cr, dashes, 2, 0.0);
                cairo_stroke(cr);
                cairo_restore(cr);
            }

            if (sample->is_major) {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x+6.0, y);
                cairo_stroke(cr);

                cairo_text_extents(cr, sample->label, &txt_ext);
                if (txt_ext.width > txt_max_width) txt_max_width = txt_ext.width;
                cairo_move_to(cr, x-txt_ext.width-txt_hei, y+txt_hei/2.0);
                cairo_show_text(cr, sample->label);
            }
            else {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x+3.0, y);
                cairo_stroke(cr);
            }
        }
    }
    if (priv->elements & SLOPE_AXIS_TITLE) {
        cairo_save(cr);
        cairo_rotate(cr, -1.5707963267948967);
        cairo_text_extents(cr, item_priv->name, &txt_ext);
        x = - fig_rect.y - (fig_rect.height + txt_ext.width)/2.0;
        y = fig_rect.x - txt_max_width - 2.0*txt_hei - 4.0;
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, item_priv->name);
        cairo_restore(cr);
    }
}


static void _slope_axis_draw_right (slope_item_t *self, cairo_t *cr)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    slope_item_private_t *item_priv = SLOPE_ITEM_PRIVATE(priv);
    cairo_text_extents_t txt_ext;
    slope_list_t *list;
    slope_iterator_t *iter;
    slope_rect_t fig_rect;
    slope_rect_t dat_rect;
    double x, y, txt_hei;
    double txt_max_width;

    /* work around crazy text heights when using unicode */
    cairo_text_extents(cr, "dummy", &txt_ext);
    txt_hei = txt_ext.height * 0.66;
    txt_max_width = 0.0;

    slope_scale_get_figure_rect(item_priv->scale, &fig_rect);
    slope_scale_get_data_rect(item_priv->scale, &dat_rect);
    x = fig_rect.x + fig_rect.width;
    y = fig_rect.y + fig_rect.height;
    
    if (priv->elements & SLOPE_AXIS_LINE) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y-fig_rect.height);
        cairo_stroke(cr);
    }
    
    if ((priv->elements & SLOPE_AXIS_TICKS) && dat_rect.height > 0.0) {
        if (slope_sampler_get_mode(priv->sampler) == SLOPE_SAMPLER_AUTO_DECIMAL)
            slope_sampler_auto_sample_decimal(priv->sampler, dat_rect.y, dat_rect.y+dat_rect.height,
                                              fig_rect.height/60.0);
        list = slope_sampler_get_sample_list(priv->sampler);

        SLOPE_LIST_FOREACH (iter, list) {
            slope_sample_t *sample = SLOPE_SAMPLE(slope_iterator_data(iter));
            slope_point_t fig_pt;
            slope_point_t dat_pt;

            if (sample->value < dat_rect.y ||
                sample->value > dat_rect.y + dat_rect.height) {
                continue;
            }

            dat_pt.x = 42.0;
            dat_pt.y = sample->value;
            slope_scale_map(item_priv->scale, &fig_pt, &dat_pt);
            y = fig_pt.y;

            if (sample->is_major) {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x-6.0, y);
                cairo_stroke(cr);

                cairo_text_extents(cr, sample->label, &txt_ext);
                if (txt_ext.width > txt_max_width) txt_max_width = txt_ext.width;
                cairo_move_to(cr, x+txt_hei, y+txt_hei/2.0);
                cairo_show_text(cr, sample->label);
            }
            else {
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x-3.0, y);
                cairo_stroke(cr);
            }
        }
    }
    if (priv->elements & SLOPE_AXIS_TITLE) {
        cairo_save(cr);
        cairo_rotate(cr, -1.5707963267948967);
        cairo_text_extents(cr, item_priv->name, &txt_ext);
        x = - fig_rect.y - (fig_rect.height + txt_ext.width)/2.0;
        y = fig_rect.x + fig_rect.width + txt_max_width + 3.0*txt_hei + 4.0;
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, item_priv->name);
        cairo_restore(cr);
    }
}


slope_sampler_t* slope_axis_get_sampler (const slope_item_t *self)
{
    return SLOPE_AXIS_GET_PRIVATE(self)->sampler;
}


slope_axis_position_t slope_axis_get_position(const slope_item_t *self)
{
    return SLOPE_AXIS_GET_PRIVATE(self)->pos;
}

slope_axis_element_t slope_axis_get_elements(const slope_item_t *self)
{
    return SLOPE_AXIS_GET_PRIVATE(self)->elements;
}


void slope_axis_set_elements(slope_item_t *self, int elements)
{
    SLOPE_AXIS_GET_PRIVATE(self)->elements = (slope_axis_element_t) elements;
}


void slope_axis_set_colors (slope_item_t *self, slope_color_t stroke_color, slope_color_t extra_color)
{
    slope_axis_private_t *priv = SLOPE_AXIS_GET_PRIVATE(self);
    priv->color = stroke_color;
}

/* slope/axis.c */
