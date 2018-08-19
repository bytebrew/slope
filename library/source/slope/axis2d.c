/*
 * Copyright (C) 2018  Elvis Teixeira
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

#include "slope/axis2d.h"
#include "slope/frame.h"
#include "slope/item.h"

typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))


struct _SlopeAxis2DPrivate
{
    double data_margin;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;
    double fig_width, fig_height;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
    double dat_width, dat_height;
};


#define SLOPE_AXIS2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))


G_DEFINE_TYPE_WITH_PRIVATE (SlopeAxis2D, slope_axis2d, SLOPE_TYPE_FRAME)


/* local decls */
static void axis2d_finalize (GObject *self);
static void axis2d_dispose (GObject *self);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc);


static void
slope_axis2d_class_init (SlopeAxis2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = axis2d_dispose;
    gobject_class->finalize = axis2d_finalize;

    item_class->draw_self = axis2d_draw_self;
    item_class->draw_tree = axis2d_draw_tree;

    klass->add_plot = axis2d_add_plot;
}


static void
slope_axis2d_init (SlopeAxis2D *axis)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (axis);

    slope_axis2d_update_scale (axis);

    m->data_margin = 16.0;
}


static void
axis2d_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->dispose (object);
}


static void
axis2d_finalize (GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->finalize (object);
}


SlopeItem*
slope_axis2d_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_AXIS2D, NULL));
}


static void
axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    /* draw the frame stuff */
    SLOPE_ITEM_CLASS (slope_axis2d_parent_class)->draw_self (self, dc);
}


static void
axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot)
{
    slope_item_append (SLOPE_ITEM (self), SLOPE_ITEM (plot));
    slope_axis2d_update_scale (self);
}


void
slope_axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot)
{
    g_return_if_fail (SLOPE_IS_AXIS2D (self));
    g_return_if_fail (SLOPE_IS_PLOT2D (plot));
    SLOPE_AXIS2D_GET_CLASS (self)->add_plot (self, plot);
}


void
slope_axis2d_update_scale (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m;
    SlopeTree *iter;
    SlopeItem *item;
    double plt_x_min, plt_x_max;
    double plt_y_min, plt_y_max;

    g_return_if_fail (SLOPE_IS_AXIS2D (self));
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    /* init values for the 'no child' case */
    m->dat_x_min = 0.0;
    m->dat_x_max = 1.0;
    m->dat_y_min = 0.0;
    m->dat_y_max = 1.0;
    m->dat_width = 1.0;
    m->dat_height = 1.0;

    if (!(iter = slope_item_get_fisrt_child (SLOPE_ITEM (self)))) {
        /* no subitems */
        return;
    }

    /* init axis extents with the first plot's extents */
    item = slope_item_from_tree_node (iter);
    slope_plot2d_get_data_extents (
                SLOPE_PLOT2D (item),
                &plt_x_min, &plt_x_max,
                &plt_y_min, &plt_y_max);

    m->dat_x_min = plt_x_min;
    m->dat_x_max = plt_x_max;
    m->dat_y_min = plt_y_min;
    m->dat_y_max = plt_y_max;

    iter = iter->next;
    while (iter != NULL) {

        /* update axis extents with the others */
        item = slope_item_from_tree_node (iter);
        slope_plot2d_get_data_extents (
                SLOPE_PLOT2D (item),
                &plt_x_min, &plt_x_max,
                &plt_y_min, &plt_y_max);

        if (plt_x_min < m->dat_x_min) m->dat_x_min = plt_x_min;
        if (plt_x_max > m->dat_x_max) m->dat_x_max = plt_x_max;
        if (plt_y_min < m->dat_y_min) m->dat_y_min = plt_y_min;
        if (plt_y_max > m->dat_y_max) m->dat_y_max = plt_y_max;

        iter = iter->next;
    }

    m->dat_width = m->dat_x_max - m->dat_x_min;
    m->dat_height = m->dat_y_max - m->dat_y_min;
}


void slope_axis2d_map (SlopeAxis2D *self, SlopePoint *f, const SlopePoint *d)
{
    SlopeAxis2DPrivate *m;

    g_return_if_fail (SLOPE_IS_AXIS2D (self));
    g_return_if_fail (d != NULL && f != NULL);
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    f->x = m->fig_x_min + ((d->x - m->dat_x_min) / m->dat_width) * m->fig_width;
    f->y = m->fig_y_min + ((m->dat_y_max - d->y) / m->dat_height) * m->fig_height;
}

void slope_axis2d_unmap (SlopeAxis2D *self, SlopePoint *d, const SlopePoint *f)
{
     SlopeAxis2DPrivate *m;

    g_return_if_fail (SLOPE_IS_AXIS2D (self));
    g_return_if_fail (d != NULL && f != NULL);
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    d->x = m->dat_x_min + ((f->x - m->fig_x_min) / m->fig_width) * m->dat_width;
    d->y = m->dat_y_min + ((m->fig_y_max - d->y) / m->fig_height) * m->dat_height;
}


static void
axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc)
{
    SlopeAxis2DPrivate *axis_p = SLOPE_AXIS2D_GET_PRIVATE (self);
    int margin = slope_frame_get_margin (SLOPE_FRAME (self));
    SlopeRect orig_rect = dc->rect;

    cairo_save (dc->cr);

    /* If stuff like margins are to be applyed, we need to
     * adjust the rectangle to which subitems will have access to */
    if (margin > 0) {
        cairo_translate (dc->cr, margin, margin);
        dc->rect.width -= 2 * margin;
        dc->rect.height -= 2 * margin;
    }

    slope_cairo_rect (dc->cr, &dc->rect);
    cairo_clip (dc->cr);

    axis_p->fig_x_min = dc->rect.x + axis_p->data_margin;
    axis_p->fig_x_max = dc->rect.x + dc->rect.width - axis_p->data_margin;
    axis_p->fig_width = axis_p->fig_x_max - axis_p->fig_x_min;

    axis_p->fig_y_min = dc->rect.y + axis_p->data_margin;
    axis_p->fig_y_max = dc->rect.y + dc->rect.height - axis_p->data_margin;
    axis_p->fig_height = axis_p->fig_y_max - axis_p->fig_y_min;

    SLOPE_ITEM_GET_CLASS (self)->draw_self (self, dc);
    SLOPE_ITEM_GET_CLASS (self)->draw_children (self, dc);

    cairo_restore (dc->cr);
    dc->rect = orig_rect;
}

/* slope/axis2d.c */
