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

#include <slope/figure_p.h>
#include <slope/scale_p.h>
#include <slope/item_p.h>
#include <slope/scale.h>
#include <stdlib.h>
#include <string.h>


static void _slope_figure_draw (slope_figure_t*, const slope_rect_t*, cairo_t*);
static void _slope_figure_set_color_scheme (slope_figure_t*, slope_color_t, slope_color_t, slope_color_t);


static slope_figure_class_t* _slope_figure_get_class ()
{
    static slope_figure_class_t figure_class;
    static slope_bool_t first_call = SLOPE_TRUE;
    
    if (first_call) {
        figure_class.init = slope_figure_init;
        figure_class.finalize = slope_figure_finalize;
        figure_class.draw = _slope_figure_draw;
        figure_class.set_color_scheme = _slope_figure_set_color_scheme;
        first_call = SLOPE_FALSE;
    }
    return &figure_class;
}


slope_figure_t* slope_figure_new (const char *name)
{
    slope_figure_t *self = SLOPE_ALLOC(slope_figure_t);
    
    self->_class = _slope_figure_get_class();
    SLOPE_FIGURE_GET_CLASS(self)->init(self);
    
    slope_figure_set_name(self, name);
    return self;
}


void slope_figure_init (slope_figure_t *self)
{
    slope_figure_private_t *priv = SLOPE_ALLOC(slope_figure_private_t);
    
    self->_private = priv;
    priv->scale_list = slope_list_new();
    priv->legend = slope_legend_new(self);
    priv->show_title = SLOPE_TRUE;
    priv->back_color = SLOPE_WHITE;
    priv->name_color = SLOPE_BLACK;
    priv->name = NULL;
}


void slope_figure_destroy (slope_figure_t *self)
{
    if (self == NULL)
        return;
    SLOPE_FIGURE_GET_CLASS(self)->finalize(self);
    SLOPE_FREE(self);
}


void slope_figure_finalize (slope_figure_t *self)
{
    slope_figure_private_t *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    if (priv == NULL)
        return;
    slope_list_destroy(priv->scale_list);
    slope_item_destroy(priv->legend);
    if (priv->name)
        SLOPE_FREE(priv->name);
    SLOPE_FREE(priv);
}


void slope_figure_add_scale (slope_figure_t *self, slope_scale_t *scale)
{
    slope_figure_private_t *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    if (scale == NULL)
        return;
    if (slope_list_size(priv->scale_list) == 0) {
        priv->ref_scale = scale;
        _slope_item_set_scale(priv->legend, scale);
    }

    slope_list_append(priv->scale_list, scale);
    _slope_scale_set_figure(scale, self);
}


void slope_figure_draw (slope_figure_t *self, const slope_rect_t *rect, cairo_t *cr)
{
    SLOPE_FIGURE_GET_CLASS(self)->draw(self, rect, cr);
}


static void _slope_figure_draw (slope_figure_t *self, const slope_rect_t *rect, cairo_t *cr)
{
    slope_figure_private_t *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    slope_iterator_t *scale_iter;
    cairo_text_extents_t txt_ext;
    
    /* clip to avoid drawing outside target rect */
    slope_rect_copy(&priv->rect, rect);
    cairo_save(cr);
    cairo_new_path(cr);
    slope_cairo_rect(cr, rect);
    slope_cairo_set_color(cr, priv->back_color);
    cairo_fill(cr);

    /* set default font */
    #if defined(_WIN32) || defined(__MINGW32__)
    cairo_select_font_face(cr, "Times New Roman",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14);
    #else
    cairo_select_font_face(cr, "sans",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);
    #endif
    
    /* draw data contents */
    SLOPE_LIST_FOREACH (scale_iter, priv->scale_list) {
        slope_scale_t *scale = SLOPE_SCALE(slope_iterator_data(scale_iter));
        if (slope_scale_get_visible(scale)) {
            _slope_scale_draw(scale, rect, cr);
        }
    }
    /* draw legend */
    if (slope_item_get_visible(priv->legend)
        && priv->ref_scale != NULL)
    {
        _slope_item_draw(priv->legend, cr);
    }
    /* draw figure title */
    if (priv->show_title) {
        cairo_text_extents(cr, priv->name, &txt_ext);
        cairo_move_to(cr, rect->x + (rect->width-txt_ext.width)/2.0, txt_ext.height+4.0);
        slope_cairo_set_color(cr, priv->name_color);
        cairo_show_text(cr, priv->name);
    }
    cairo_restore(cr);
}


void slope_figure_write_to_png (slope_figure_t *self,
                                const char *filename,
                                int width, int height)
{
    cairo_surface_t *image =
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                width, height);
    cairo_t *cr = cairo_create(image);
    slope_rect_t rect;
    slope_rect_set(&rect, 0.0, 0.0, (double) width, (double) height);
    slope_figure_draw(self, &rect, cr);
    cairo_surface_write_to_png(image, filename);
    cairo_surface_destroy(image);
    cairo_destroy(cr);
}


void slope_figure_set_name (slope_figure_t *self, const char *name)
{
    slope_figure_private_t *priv;

    if (self == NULL)
        return;

    priv = SLOPE_FIGURE_GET_PRIVATE(self);
    if (priv->name != NULL)
        SLOPE_FREE(priv->name);
    priv->name = strdup(name);
}


slope_list_t* slope_figure_get_scale_list (const slope_figure_t *self)
{
    if (self == NULL)
        return NULL;
    return SLOPE_FIGURE_GET_PRIVATE(self)->scale_list;
}


const char* slope_figure_get_name (const slope_figure_t *self)
{
    if (self == NULL)
        return NULL;
    return SLOPE_FIGURE_GET_PRIVATE(self)->name;
}


void slope_figure_get_rect (const slope_figure_t *self, slope_rect_t *rect)
{
    slope_figure_private_t *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    slope_rect_copy(rect, &priv->rect);
}


slope_scale_t* slope_figure_get_reference_scale (const slope_figure_t *self)
{
    return SLOPE_FIGURE_GET_PRIVATE(self)->ref_scale;
}


slope_item_t* slope_figure_get_legend (const slope_figure_t *self)
{
    return SLOPE_FIGURE_GET_PRIVATE(self)->legend;
}


void slope_figure_set_color_scheme (slope_figure_t *self, slope_color_t background,
                                    slope_color_t foreground, slope_color_t extra_color)
{
    SLOPE_FIGURE_GET_CLASS(self)->set_color_scheme(self, background, foreground, extra_color);
}


static void _slope_figure_set_color_scheme (slope_figure_t *self, slope_color_t background,
                                            slope_color_t foreground, slope_color_t extra_color)
{
    slope_figure_private_t *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    slope_iterator_t *iter;

    priv->back_color = background;
    priv->name_color = foreground;
    slope_legend_set_colors(priv->legend, foreground, background);
    SLOPE_LIST_FOREACH (iter, priv->scale_list)
        slope_scale_set_color_scheme (SLOPE_SCALE(slope_iterator_data(iter)),
            background, foreground, extra_color);
}

/* slope/figure.c */