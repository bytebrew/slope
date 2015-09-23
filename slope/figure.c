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

#include "slope/figure_p.h"
#include "slope/metrics_p.h"
#include "slope/xymetrics.h"
#include "slope/legend_p.h"
#include "slope/item.h"
#include "slope/list.h"
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>


static slope_object_class_t*
_slope_figure_get_class ()
{
    static slope_object_class_t klass;
    static int first_call = SLOPE_TRUE;

    if (first_call == SLOPE_TRUE) {
        klass.destroy_fn = _slope_figure_destroy;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}


slope_figure_t*
slope_figure_create()
{
    slope_figure_t *self = malloc(sizeof(slope_figure_t));
    slope_figure_private_t *data = malloc(sizeof(slope_figure_private_t));
    slope_object_t *object = (slope_object_t*) self;

    object->klass = _slope_figure_get_class();
    object->priv = (slope_object_private_t*) data;

    data->metrics = NULL;
    data->default_metrics = NULL;
    data->change_callback = NULL;
    data->legend = slope_legend_create();
    data->default_font = slope_font_create ("Sans", 10);
    data->fill_back = SLOPE_TRUE;
    slope_color_set_name (&data->back_color, SLOPE_WHITE);

    return self;
}


void
slope_figure_destroy (slope_figure_t *figure)
{
    slope_object_destroy ((slope_object_t*) figure);
}


void
_slope_figure_destroy (slope_object_t *object)
{
    slope_figure_t *self = (slope_figure_t*) object;
    slope_figure_private_t *data = SLOPE_FIGURE_GET_PRIVATE(object);

    slope_item_destroy (data->legend);
    slope_list_destroy (data->metrics);
    slope_font_destroy (data->default_font);
    free (data);
}


void
slope_figure_add_metrics (slope_figure_t *self,
                          slope_metrics_t *metrics)
{
    slope_figure_private_t *data;

    if (self == NULL) return;
    if (metrics == NULL) return;

    data = SLOPE_FIGURE_GET_PRIVATE(self);

    _slope_metrics_set_figure(metrics, self);
    data->metrics = slope_list_append(data->metrics, metrics);
    data->default_metrics = metrics;

    if (data->change_callback) {
        data->change_callback(self);
    }
}


slope_list_t*
slope_figure_get_metrics_list (const slope_figure_t *self)
{
    slope_figure_private_t *data;

    if (self == NULL) return NULL;

    data = SLOPE_FIGURE_GET_PRIVATE(self);
    return data->metrics;
}


void
slope_figure_draw (slope_figure_t *self, cairo_t *cr,
                   const slope_rect_t *rect)
{
    slope_iterator_t *met_iter;
    slope_figure_private_t *data;

    data = SLOPE_FIGURE_GET_PRIVATE(self);

    /* perform any pending drawing and clip to the figure's
       rectangle */
    cairo_save(cr);
    cairo_new_path(cr);
    slope_cairo_rectangle(cr, rect);
    cairo_clip(cr);

    /* fill background if required */
    if (data->fill_back) {
        slope_cairo_set_color(cr, &data->back_color);
        cairo_paint(cr);
    }

    /* draw main items */
    SLOPE_LIST_FOREACH (met_iter, data->metrics) {
        slope_metrics_t *metrics;
        metrics = (slope_metrics_t*) slope_iterator_data(met_iter);

        if (slope_metrics_get_visible(metrics)) {
            _slope_metrics_draw(metrics, cr, rect);
        }
    }

    /* draw legend */
    if (slope_item_get_visible(data->legend)
            && data->default_metrics != NULL)
    {
        _slope_legend_draw(data->legend, cr, data->default_metrics);
    }
    cairo_restore(cr);
}


int
slope_figure_write_to_png (
           slope_figure_t *self,
           const char *filename,
           int width, int height)
{
    cairo_surface_t *surf;
    cairo_t *cr;
    slope_rect_t rect;

    if (self == NULL) return;

    surf = cairo_image_surface_create(
            CAIRO_FORMAT_ARGB32, width, height);
    if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
        return SLOPE_ERROR;
    }

    cr = cairo_create(surf);
    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        return SLOPE_ERROR;
    }

    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_figure_draw(self, cr, &rect);
    cairo_surface_write_to_png(surf, filename);

    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        cairo_destroy(cr);
        return SLOPE_ERROR;
    }

    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_SUCCESS;
}


int
slope_figure_write_to_svg (
        slope_figure_t *self,
        const char *filename,
        int width, int height)
{
    cairo_surface_t *surf;
    cairo_t *cr;
    slope_rect_t rect;

    if (self == NULL) return;

    surf = cairo_svg_surface_create(
            filename, width, height);
    if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
        return SLOPE_ERROR;
    }

    cr = cairo_create(surf);
    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        return SLOPE_ERROR;
    }

    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_figure_draw(self, cr, &rect);

    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        cairo_destroy(cr);
        return SLOPE_ERROR;
    }

    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_SUCCESS;
}


int
slope_figure_write_to_pdf (
        slope_figure_t *self,
        const char *filename,
        int width, int height)
{
    cairo_surface_t *surf;
    cairo_t *cr;
    slope_rect_t rect;

    if (self == NULL) return;

    surf = cairo_pdf_surface_create(filename, width, height);
    if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
        return SLOPE_ERROR;
    }

    cr = cairo_create(surf);
    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        return SLOPE_ERROR;
    }

    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_figure_draw(self, cr, &rect);

    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        cairo_destroy(cr);
        return SLOPE_ERROR;
    }

    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_SUCCESS;
}


int
slope_figure_write_to_ps (
        slope_figure_t *self,
        const char *filename,
        int width, int height)
{
    cairo_surface_t *surf;
    cairo_t *cr;
    slope_rect_t rect;

    if (self == NULL) return;

    surf = cairo_ps_surface_create(
            filename, width, height);
    if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
        return SLOPE_ERROR;
    }

    cr = cairo_create(surf);
    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        return SLOPE_ERROR;
    }

    slope_rect_set(&rect, 0.0, 0.0, width, height);
    slope_figure_draw(self, cr, &rect);

    if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(surf);
        cairo_destroy(cr);
        return SLOPE_ERROR;
    }

    cairo_surface_destroy(surf);
    cairo_destroy(cr);
    return SLOPE_SUCCESS;
}


slope_metrics_t*
slope_figure_get_default_metrics (slope_figure_t *self)
{
    slope_figure_private_t *data;

    if (self == NULL) return NULL;

    data = SLOPE_FIGURE_GET_PRIVATE(self);
    return data->default_metrics;
}


void
slope_figure_set_change_callback (
        slope_figure_t *self,
        slope_callback_t callback)
{
    slope_figure_private_t *data;

    if (self == NULL) return;

    data = SLOPE_FIGURE_GET_PRIVATE(self);
    data->change_callback = callback;
}


void
slope_figure_notify_appearence_change (
        slope_figure_t *self,
        slope_item_t *item)
{
    slope_figure_private_t *data;

    if (self == NULL) return;
    (void) item; /* reserved for possible future use */

    data = SLOPE_FIGURE_GET_PRIVATE(self);
    
    if (data->change_callback != NULL) {
        data->change_callback(self);
    }
}


void
slope_figure_notify_data_change (
        slope_figure_t *self,
        slope_item_t *item)
{
    slope_metrics_t *metrics;
    slope_figure_private_t *data;

    if (self == NULL) return;

    data = SLOPE_FIGURE_GET_PRIVATE(self);
    metrics = slope_item_get_metrics(item);
    
    slope_metrics_update(metrics);
    
    if (data->change_callback != NULL) {
        data->change_callback(self);
    }
}


void
slope_figure_track_region (
        slope_figure_t *self,
        double x1, double y1,
        double x2, double y2)
{
    slope_figure_private_t *data;
    slope_iterator_t *metr_iter;

    if (self == NULL) return;

    data = SLOPE_FIGURE_GET_PRIVATE(self);

    if (x2 < x1) {
        double tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    if (y2 > y1) {
        double tmp = y2;
        y2 = y1;
        y1 = tmp;
    }

    SLOPE_LIST_FOREACH (metr_iter, data->metrics) {
        slope_metrics_t *metrics;
        metrics = slope_iterator_data(metr_iter);

        /* cartesian coordinates (xymetrics) */
        if (slope_metrics_get_type(metrics) == SLOPE_XYMETRICS) {
            slope_xymetrics_set_x_range(metrics,
                    slope_xymetrics_unmap_x(metrics, x1),
                    slope_xymetrics_unmap_x(metrics, x2));
            slope_xymetrics_set_y_range(metrics,
                    slope_xymetrics_unmap_y(metrics, y1),
                    slope_xymetrics_unmap_y(metrics, y2));
        }
    }
}


void
slope_figure_update (slope_figure_t *self)
{
    slope_figure_private_t *data;
    slope_iterator_t *metr_iter;

    if (self == NULL) return;

    data = SLOPE_FIGURE_GET_PRIVATE(self);

    SLOPE_LIST_FOREACH (metr_iter, data->metrics) {
        slope_metrics_t *metrics;
        metrics = (slope_metrics_t*) slope_iterator_data(metr_iter);
        if (slope_metrics_get_visible(metrics)) {
            slope_metrics_update(metrics);
        }
    }
}


slope_font_t*
slope_figure_get_default_font (slope_figure_t *self)
{
    slope_figure_private_t *data;

    if (self == NULL) return NULL;

    data = SLOPE_FIGURE_GET_PRIVATE(self);
    
    return data->default_font;
}

/* slope/figure.h */
