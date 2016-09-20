/*
 * Copyright (C) 2016  Elvis Teixeira
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

#include <slope/xyscale_p.h>
#include <slope/figure.h>
#include <slope/item_p.h>

#define MAX_AXIS 4


typedef struct
_SlopeXyScalePrivate
{
    SlopeItem *axis[MAX_AXIS];

    double left_margin, right_margin;
    double top_margin, bottom_margin;

    double horiz_pad, vertical_pad;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;
    double fig_width, fig_height;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
    double dat_width, dat_height;

    SlopePoint mouse_p1;
    SlopePoint mouse_p2;
    SlopeColor mouse_rect_color;
    gboolean on_drag;
}
SlopeXyScalePrivate;


#define SLOPE_XYSCALE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_XYSCALE_TYPE, SlopeXyScalePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeXyScale, slope_xyscale, SLOPE_SCALE_TYPE)



static void _xyscale_finalize (GObject *self);
static void _xyscale_draw (SlopeScale *self, const SlopeRect *rect, cairo_t *cr);
static void _xyscale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
static void _xyscale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
static void _xyscale_rescale (SlopeScale *self);
static void _xyscale_get_figure_rect (SlopeScale *self, SlopeRect *rect);
static void _xyscale_get_data_rect (SlopeScale *self, SlopeRect *rect);
static void _xyscale_position_axis (SlopeScale *self);
static gboolean _xyscale_mouse_event (SlopeScale *self, SlopeViewMouseEvent *event);


static void
slope_xyscale_class_init (SlopeXyScaleClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    SlopeScaleClass *scale_klass = SLOPE_SCALE_CLASS(klass);

    object_klass->finalize = _xyscale_finalize;

    scale_klass->draw = _xyscale_draw;
    scale_klass->map = _xyscale_map;
    scale_klass->unmap = _xyscale_unmap;
    scale_klass->rescale = _xyscale_rescale;
    scale_klass->get_data_rect = _xyscale_get_data_rect;
    scale_klass->get_figure_rect = _xyscale_get_figure_rect;
    scale_klass->mouse_event = _xyscale_mouse_event;
}


static void
slope_xyscale_init (SlopeXyScale *self)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    int k;

    priv->axis[SLOPE_XYSCALE_AXIS_BOTTOM] = slope_xyaxis_new(SLOPE_XYAXIS_HORIZONTAL);
    priv->axis[SLOPE_XYSCALE_AXIS_TOP] = slope_xyaxis_new(SLOPE_XYAXIS_HORIZONTAL);
    priv->axis[SLOPE_XYSCALE_AXIS_LEFT] = slope_xyaxis_new(SLOPE_XYAXIS_VERTICAL);
    priv->axis[SLOPE_XYSCALE_AXIS_RIGHT] = slope_xyaxis_new(SLOPE_XYAXIS_VERTICAL);

    slope_xyaxis_set_components(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_BOTTOM]),
                                SLOPE_XYAXIS_LINE
                                |SLOPE_XYAXIS_TICKS_DOWN
                                |SLOPE_XYAXIS_TITLE);
    slope_xyaxis_set_components(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_LEFT]),
                                SLOPE_XYAXIS_LINE
                                |SLOPE_XYAXIS_TICKS_DOWN
                                |SLOPE_XYAXIS_TITLE);
    slope_xyaxis_set_components(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_TOP]),
                                SLOPE_XYAXIS_LINE);
    slope_xyaxis_set_components(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_RIGHT]),
                                SLOPE_XYAXIS_LINE);

    for (k=0; k<MAX_AXIS; ++k) _item_set_scale(priv->axis[k], SLOPE_SCALE(self));

    priv->left_margin = 60.0;
    priv->right_margin = 15.0;
    priv->top_margin = 15.0;
    priv->bottom_margin = 30.0;
    slope_scale_set_name_top_padding(SLOPE_SCALE(self), priv->top_margin + 2);

    priv->horiz_pad = 0.02;
    priv->vertical_pad = 0.03;
    priv->on_drag = FALSE;

    priv->mouse_rect_color = SLOPE_GRAY(80);

    slope_scale_rescale(SLOPE_SCALE(self));
}


static
void _xyscale_finalize (GObject *self)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    int k;

    if (priv->axis[0] != NULL) {
        for (k=0; k<MAX_AXIS; ++k) {
            g_object_unref(priv->axis[k]);
        }
        priv->axis[0] = NULL;
    }
}


SlopeScale* slope_xyscale_new ()
{
    SlopeXyScale *self = SLOPE_XYSCALE(g_object_new(SLOPE_XYSCALE_TYPE, NULL));

    return SLOPE_SCALE(self);
}


static
void _xyscale_draw (SlopeScale *self, const SlopeRect *rect, cairo_t *cr)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    int k;

    priv->fig_x_min = rect->x + priv->left_margin;
    priv->fig_x_max = rect->x + rect->width - priv->right_margin;
    priv->fig_width = priv->fig_x_max - priv->fig_x_min;

    priv->fig_y_min = rect->y + priv->top_margin;
    priv->fig_y_max = rect->y + rect->height - priv->bottom_margin;
    priv->fig_height = priv->fig_y_max - priv->fig_y_min;

    cairo_save(cr);
    cairo_new_path(cr);
    cairo_rectangle(cr, priv->fig_x_min, priv->fig_y_min,
                    priv->fig_width, priv->fig_height);
    cairo_clip(cr);

    SLOPE_SCALE_CLASS(
        g_type_class_peek_parent(
            SLOPE_XYSCALE_GET_CLASS(self)))
                ->draw(self, rect, cr);

    cairo_restore(cr);

    /* draw axis */
    _xyscale_position_axis(self);
    for (k=0; k<MAX_AXIS; ++k) {
        if (slope_item_get_is_visible(priv->axis[k]) == TRUE) {
            _item_draw(priv->axis[k], cr);
        }
    }

    if (priv->on_drag == TRUE) {
        static const double dashes[2] = { 4.0, 4.0 };
        cairo_save(cr);
        cairo_new_path(cr);
        cairo_rectangle(cr, priv->mouse_p1.x, priv->mouse_p1.y,
                        priv->mouse_p2.x - priv->mouse_p1.x,
                        priv->mouse_p2.y - priv->mouse_p1.y);
        cairo_set_dash(cr, dashes, 2, 0.0);
        cairo_set_line_width(cr, 1.0);
        slope_cairo_set_antialias(cr, FALSE);
        slope_cairo_set_color(cr, priv->mouse_rect_color);
        cairo_stroke(cr);
        cairo_restore(cr);
    }
}


static
void _xyscale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    double tmp;

    tmp = (src->x - priv->dat_x_min) / priv->dat_width;
    res->x = priv->fig_x_min + tmp * priv->fig_width;

    tmp = (src->y - priv->dat_y_min) / priv->dat_height;
    res->y = priv->fig_y_max - tmp * priv->fig_height;
}


static
void _xyscale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    double tmp;

    tmp = (src->x - priv->fig_x_min) / priv->fig_width;
    res->x = priv->dat_x_min + tmp * priv->dat_width;

    tmp = (priv->fig_y_max - src->y) / priv->fig_height;
    res->y = priv->dat_y_min + tmp * priv->dat_height;
}


static
void _xyscale_rescale (SlopeScale *self)
{
    SlopeXyScalePrivate *priv;
    GList *iter, *list;
    SlopeItem *item;
    SlopeRect item_rect;
    double padding;

    priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    list = slope_scale_get_item_list(self);

    if (list == NULL) {
        slope_xyscale_set_x_range(SLOPE_XYSCALE(self), 0.0, 1.0);
        slope_xyscale_set_y_range(SLOPE_XYSCALE(self), 0.0, 1.0);
        return;
    }

    iter = list;
    item = SLOPE_ITEM(iter->data);
    slope_item_get_data_rect(item, &item_rect);

    /* initialize to the first item to avoid junk values */
    priv->dat_x_min = item_rect.x;
    priv->dat_x_max = item_rect.x + item_rect.width;
    priv->dat_y_min = item_rect.y;
    priv->dat_y_max = item_rect.y + item_rect.height;

    while (iter != NULL) {
        item = SLOPE_ITEM(iter->data);
        slope_item_get_data_rect(item, &item_rect);
        iter = iter->next;

        if (item_rect.x < priv->dat_x_min)
            priv->dat_x_min = item_rect.x;
        if ((item_rect.x+item_rect.width) > priv->dat_x_max)
            priv->dat_x_max = (item_rect.x+item_rect.width);
        if (item_rect.y < priv->dat_y_min)
            priv->dat_y_min = item_rect.y;
        if ((item_rect.y+item_rect.height) > priv->dat_y_max)
            priv->dat_y_max = (item_rect.y+item_rect.height);
    }

    /* evaluate width and height of the data space taking
     * int account the padding */
    priv->dat_width = priv->dat_x_max - priv->dat_x_min;
    padding = priv->dat_width * priv->horiz_pad;
    priv->dat_x_min -= padding;
    priv->dat_x_max += padding;
    priv->dat_width += 2.0*padding;

    priv->dat_height = priv->dat_y_max - priv->dat_y_min;
    padding = priv->dat_height * priv->vertical_pad;
    priv->dat_y_min -= padding;
    priv->dat_y_max += padding;
    priv->dat_height += 2.0*padding;
}


static
void _xyscale_get_figure_rect (SlopeScale *self, SlopeRect *rect)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    rect->x = priv->fig_x_min;
    rect->y = priv->fig_y_min;
    rect->width = priv->fig_width;
    rect->height = priv->fig_height;
}


static
void _xyscale_get_data_rect (SlopeScale *self, SlopeRect *rect)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    rect->x = priv->dat_x_min;
    rect->y = priv->dat_y_min;
    rect->width = priv->dat_width;
    rect->height = priv->dat_height;
}


static
void _xyscale_position_axis (SlopeScale *self)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    slope_xyaxis_set_position(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_BOTTOM]),
            priv->dat_x_min, priv->dat_x_max, priv->dat_y_min);
    slope_xyaxis_set_position(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_TOP]),
            priv->dat_x_min, priv->dat_x_max, priv->dat_y_max);
    slope_xyaxis_set_position(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_LEFT]),
            priv->dat_y_min, priv->dat_y_max, priv->dat_x_min);
    slope_xyaxis_set_position(SLOPE_XYAXIS(priv->axis[SLOPE_XYSCALE_AXIS_RIGHT]),
            priv->dat_y_min, priv->dat_y_max, priv->dat_x_max);
}


void slope_xyscale_set_visible_axis (SlopeXyScale *self, int axis_flag)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    if (axis_flag == SLOPE_XYSCALE_NO_AXIS) {
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_BOTTOM], FALSE);
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_LEFT], FALSE);
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_TOP], FALSE);
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_RIGHT], FALSE);
    }
    else if (axis_flag == SLOPE_XYSCALE_FRAME_AXIS) {
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_BOTTOM], TRUE);
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_LEFT], TRUE);
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_TOP], TRUE);
        slope_item_set_is_visible(priv->axis[SLOPE_XYSCALE_AXIS_RIGHT], TRUE);
    }
}


SlopeItem* slope_xyscale_get_axis (SlopeXyScale *self, int axis_id)
{
    return SLOPE_XYSCALE_GET_PRIVATE(self)->axis[axis_id];
}


void slope_xyscale_set_x_range (SlopeXyScale *self, double min, double max)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    priv->dat_x_min = min;
    priv->dat_x_max = max;
    priv->dat_width = max - min;
}


void slope_xyscale_set_y_range (SlopeXyScale *self, double min, double max)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);

    priv->dat_y_min = min;
    priv->dat_y_max = max;
    priv->dat_height = max - min;
}


static
gboolean _xyscale_mouse_event (SlopeScale *self,
                               SlopeViewMouseEvent *event)
{
    SlopeXyScalePrivate *priv = SLOPE_XYSCALE_GET_PRIVATE(self);
    SlopeView *view = slope_scale_get_view(self);

    if (event->type == SLOPE_VIEW_BUTTON_PRESS) {
        if (event->buttom == SLOPE_VIEW_LEFT_BUTTON) {
            priv->mouse_p1.x = event->x;
            priv->mouse_p1.y = event->y;
            priv->mouse_p2 = priv->mouse_p1;
            priv->on_drag = TRUE;
        }
        else if (event->buttom == SLOPE_VIEW_RIGHT_BUTTON) {
            slope_scale_rescale(self);
            slope_view_redraw(view);
        }
    }

    if (event->type == SLOPE_VIEW_MOVE_PRESSED &&
            priv->on_drag == TRUE) {

        priv->mouse_p2.x = event->x;
        priv->mouse_p2.y = event->y;
        slope_view_redraw(view);
    }

    if (event->type == SLOPE_VIEW_BUTTON_RELEASE) {
        priv->on_drag = FALSE;
        if (event->buttom == SLOPE_VIEW_LEFT_BUTTON) {
            SlopePoint data_p1, data_p2;

            if (priv->mouse_p2.x < priv->mouse_p1.x) {
                double tmp = priv->mouse_p1.x;
                priv->mouse_p1.x = priv->mouse_p2.x;
                priv->mouse_p2.x = tmp;
            }

            if (priv->mouse_p2.y < priv->mouse_p1.y) {
                double tmp = priv->mouse_p1.y;
                priv->mouse_p1.y = priv->mouse_p2.y;
                priv->mouse_p2.y = tmp;
            }

            if (SLOPE_ABS(priv->mouse_p1.x - priv->mouse_p2.x) > 3 &&
                    SLOPE_ABS(priv->mouse_p1.y - priv->mouse_p2.y) > 3) {

                slope_scale_unmap(self, &data_p1, &priv->mouse_p1);
                slope_scale_unmap(self, &data_p2, &priv->mouse_p2);

                slope_xyscale_set_x_range(SLOPE_XYSCALE(self), data_p1.x, data_p2.x);
                slope_xyscale_set_y_range(SLOPE_XYSCALE(self), data_p2.y, data_p1.y);
            }

            slope_view_redraw(view);
        }
    }

    return FALSE;
}

/* slope/xyscale.c */
