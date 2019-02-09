/*
 * Copyright (C) 2019  Elvis Teixeira
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
#include "slope/sampler.h"

typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))


struct _SlopeAxis2DPrivate
{
    double left_margin, right_margin;
    double bottom_margin, top_margin;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;
    double fig_width, fig_height;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
    double dat_width, dat_height;

    SlopeItem *bottom_scale;
    SlopeItem *left_scale;
    SlopeItem *top_scale;
    SlopeItem *right_scale;
    SlopeItem *x_scale;
    SlopeItem *y_scale;
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
static void axis2d_set_scales_position (SlopeAxis2D *self);

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
slope_axis2d_init (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);

    slope_axis2d_update_scale (self);

    m->right_margin = 20.0;
    m->top_margin = 20.0;
    m->bottom_margin = 20.0;
    m->left_margin = 20.0;

    m->bottom_scale = slope_scale_new();
    slope_item_append (SLOPE_ITEM (self), m->bottom_scale);

    m->left_scale = slope_scale_new();
    slope_item_append (SLOPE_ITEM (self), m->left_scale);

    m->top_scale = slope_scale_new();
    slope_item_append (SLOPE_ITEM (self), m->top_scale);

    m->right_scale = slope_scale_new();
    slope_item_append (SLOPE_ITEM (self), m->right_scale);

    m->x_scale = slope_scale_new();
    slope_item_append (SLOPE_ITEM (self), m->x_scale);

    m->y_scale = slope_scale_new();
    slope_item_append (SLOPE_ITEM (self), m->y_scale);

    slope_axis2d_set_scales (SLOPE_AXIS2D (self),
                             SLOPE_AXIS2D_SCALE_X | SLOPE_AXIS2D_SCALE_Y);
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
slope_axis2d_new (const char *title)
{
    SlopeItem *self = SLOPE_ITEM (g_object_new (SLOPE_TYPE_AXIS2D, NULL));
    slope_frame_set_title (SLOPE_FRAME (self), title);
    return self;
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
    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (SLOPE_IS_PLOT2D (plot));
    SLOPE_AXIS2D_GET_CLASS (self)->add_plot (self, plot);
}


SlopeScale* slope_axis2d_get_scale (SlopeAxis2D *self, SlopeAxisScale scale)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    g_assert (SLOPE_IS_AXIS2D (self));

    switch (scale) {
        case SLOPE_AXIS2D_SCALE_BOTTOM: return SLOPE_SCALE(m->bottom_scale);
        case SLOPE_AXIS2D_SCALE_LEFT: return SLOPE_SCALE(m->left_scale);
        case SLOPE_AXIS2D_SCALE_TOP: return SLOPE_SCALE(m->top_scale);
        case SLOPE_AXIS2D_SCALE_RIGHT: return SLOPE_SCALE(m->right_scale);
        case SLOPE_AXIS2D_SCALE_X: return SLOPE_SCALE(m->x_scale);
        case SLOPE_AXIS2D_SCALE_Y: return SLOPE_SCALE(m->y_scale);
    }

    g_assert (FALSE);
    return NULL;
}


void slope_axis2d_set_scales (SlopeAxis2D *self, SlopeAxisScale scales)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);

    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (scales >= 0
              && scales <= (SLOPE_AXIS2D_SCALE_BOTTOM
                            |SLOPE_AXIS2D_SCALE_LEFT
                            |SLOPE_AXIS2D_SCALE_TOP
                            |SLOPE_AXIS2D_SCALE_RIGHT
                            |SLOPE_AXIS2D_SCALE_X
                            |SLOPE_AXIS2D_SCALE_Y));

    slope_item_set_visible (m->bottom_scale, slope_enabled(scales, SLOPE_AXIS2D_SCALE_BOTTOM));
    slope_item_set_visible (m->left_scale, slope_enabled(scales, SLOPE_AXIS2D_SCALE_LEFT));
    slope_item_set_visible (m->top_scale, slope_enabled(scales, SLOPE_AXIS2D_SCALE_TOP));
    slope_item_set_visible (m->right_scale, slope_enabled(scales, SLOPE_AXIS2D_SCALE_RIGHT));
    slope_item_set_visible (m->x_scale, slope_enabled(scales, SLOPE_AXIS2D_SCALE_X));
    slope_item_set_visible (m->y_scale, slope_enabled(scales, SLOPE_AXIS2D_SCALE_Y));
}


void
slope_axis2d_update_scale (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m;
    SlopeTree *iter;
    SlopeItem *item;
    double plt_x_min, plt_x_max;
    double plt_y_min, plt_y_max;

    g_assert (SLOPE_IS_AXIS2D (self));
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

    /* Init axis extents with the first plot's extents */
    item = slope_item_from_tree_node (iter);
    while (iter != NULL) {

        item = slope_item_from_tree_node (iter);
        if (SLOPE_IS_PLOT2D(item)) {

            slope_plot2d_get_data_extents (
                    SLOPE_PLOT2D (item),
                    &plt_x_min, &plt_x_max,
                    &plt_y_min, &plt_y_max);

            m->dat_x_min = plt_x_min;
            m->dat_x_max = plt_x_max;
            m->dat_y_min = plt_y_min;
            m->dat_y_max = plt_y_max;
            iter = iter->next;
            break;
        }

        iter = iter->next;
    }

    /* Update axis extents with the others */
    while (iter != NULL) {

        item = slope_item_from_tree_node (iter);
        if (SLOPE_IS_PLOT2D(item)) {

            slope_plot2d_get_data_extents (
                    SLOPE_PLOT2D (item),
                    &plt_x_min, &plt_x_max,
                    &plt_y_min, &plt_y_max);

            if (plt_x_min < m->dat_x_min) m->dat_x_min = plt_x_min;
            if (plt_x_max > m->dat_x_max) m->dat_x_max = plt_x_max;
            if (plt_y_min < m->dat_y_min) m->dat_y_min = plt_y_min;
            if (plt_y_max > m->dat_y_max) m->dat_y_max = plt_y_max;
        }

        iter = iter->next;
    }

    m->dat_width = m->dat_x_max - m->dat_x_min;
    m->dat_height = m->dat_y_max - m->dat_y_min;
}


void
slope_axis2d_map (SlopeAxis2D *self, SlopePoint *f, const SlopePoint *d)
{
    SlopeAxis2DPrivate *m;

    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (d != NULL && f != NULL);
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    f->x = m->fig_x_min + ((d->x - m->dat_x_min) / m->dat_width) * m->fig_width;
    f->y = m->fig_y_min + ((m->dat_y_max - d->y) / m->dat_height) * m->fig_height;
}


void
slope_axis2d_unmap (SlopeAxis2D *self, SlopePoint *d, const SlopePoint *f)
{
     SlopeAxis2DPrivate *m;

    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (d != NULL && f != NULL);
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    d->x = m->dat_x_min + ((f->x - m->fig_x_min) / m->fig_width) * m->dat_width;
    d->y = m->dat_y_min + ((m->fig_y_max - d->y) / m->fig_height) * m->dat_height;
}


static void
axis2d_set_scales_position (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    SlopePoint fig_p1, fig_p2, zero;

    fig_p1.x = m->fig_x_min;
    fig_p1.y = m->fig_y_max;
    fig_p2.x = m->fig_x_max;
    fig_p2.y = m->fig_y_max;
    slope_scale_set_figure_position (SLOPE_SCALE (m->bottom_scale), &fig_p1, &fig_p2);
    slope_scale_set_data_extents (SLOPE_SCALE (m->bottom_scale), m->dat_x_min, m->dat_x_max);

    fig_p1.x = m->fig_x_min;
    fig_p1.y = m->fig_y_max;
    fig_p2.x = m->fig_x_min;
    fig_p2.y = m->fig_y_min;
    slope_scale_set_figure_position (SLOPE_SCALE (m->left_scale), &fig_p1, &fig_p2);
    slope_scale_set_data_extents (SLOPE_SCALE (m->left_scale), m->dat_y_min, m->dat_y_max);

    fig_p1.x = m->fig_x_min;
    fig_p1.y = m->fig_y_min;
    fig_p2.x = m->fig_x_max;
    fig_p2.y = m->fig_y_min;
    slope_scale_set_figure_position (SLOPE_SCALE (m->top_scale), &fig_p1, &fig_p2);
    slope_scale_set_data_extents (SLOPE_SCALE (m->top_scale), m->dat_x_min, m->dat_x_max);

    fig_p1.x = m->fig_x_max;
    fig_p1.y = m->fig_y_max;
    fig_p2.x = m->fig_x_max;
    fig_p2.y = m->fig_y_min;
    slope_scale_set_figure_position (SLOPE_SCALE (m->right_scale), &fig_p1, &fig_p2);
    slope_scale_set_data_extents (SLOPE_SCALE (m->right_scale), m->dat_y_min, m->dat_y_max);

    fig_p2.x = 0;
    fig_p2.y = 0;
    slope_axis2d_map (self, &zero, &fig_p2);

    fig_p1.x = m->fig_x_min;
    fig_p1.y = zero.y;
    fig_p2.x = m->fig_x_max;
    fig_p2.y = zero.y;
    slope_scale_set_figure_position (SLOPE_SCALE (m->x_scale), &fig_p1, &fig_p2);
    slope_scale_set_data_extents (SLOPE_SCALE (m->x_scale), m->dat_x_min, m->dat_x_max);

    fig_p1.x = zero.x;
    fig_p1.y = m->fig_y_max;
    fig_p2.x = zero.x;
    fig_p2.y = m->fig_y_min;
    slope_scale_set_figure_position (SLOPE_SCALE (m->y_scale), &fig_p1, &fig_p2);
    slope_scale_set_data_extents (SLOPE_SCALE (m->y_scale), m->dat_y_min, m->dat_y_max);

}


static void
axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    double margin = slope_frame_get_margin (SLOPE_FRAME (self));
    SlopeRect orig_rect = dc->rect;

    cairo_save (dc->cr);

    /* If stuff like margins are to be applyed, we need to
     * adjust the rectangle to which subitems will have access to */
    if (margin > 0) {
        dc->rect.x += margin;
        dc->rect.y += margin;
        dc->rect.width -= 2 * margin;
        dc->rect.height -= 2 * margin;
    }

    slope_cairo_rect (dc->cr, &dc->rect);
    cairo_clip (dc->cr);

    m->fig_x_min = dc->rect.x + m->left_margin;
    m->fig_x_max = dc->rect.x + dc->rect.width - m->right_margin;
    m->fig_width = m->fig_x_max - m->fig_x_min;

    m->fig_y_min = dc->rect.y + m->top_margin;
    m->fig_y_max = dc->rect.y + dc->rect.height - m->bottom_margin;
    m->fig_height = m->fig_y_max - m->fig_y_min;

    slope_frame_draw_rect (SLOPE_FRAME (self), dc);
    axis2d_set_scales_position (SLOPE_AXIS2D (self));
    SLOPE_ITEM_GET_CLASS (self)->draw_children (self, dc);
    slope_frame_draw_title (SLOPE_FRAME (self), dc);

    cairo_restore (dc->cr);
    dc->rect = orig_rect;
}

/* slope/axis2d.c */
