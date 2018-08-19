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
#include "slope/item_p.h"

typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))

typedef struct _AxisLine AxisLine;
#define AXIS_LINE(Addr) ((AxisLine *) (Addr))


struct _AxisLine {
    SlopeRGBA line_color;
    double line_width;
    SlopeOrientation orient;
    guint32 options;
};


struct _SlopeAxis2DPrivate
{
    AxisLine l[6];

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeAxis2D, slope_axis2d, SLOPE_TYPE_FRAME)
#define SLOPE_AXIS2D_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))

/* local decls */
static void axis2d_finalize (GObject *self);
static void axis2d_dispose (GObject *self);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot);

static void
slope_axis2d_class_init (SlopeAxis2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = axis2d_dispose;
    gobject_class->finalize = axis2d_finalize;

    item_class->draw_self = axis2d_draw_self;

    klass->add_plot = axis2d_add_plot;
}


static void
slope_axis2d_init (SlopeAxis2D *axis)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (axis);

    /* metic lines initialization */
    m->l[SLOPE_AXIS2D_BOTTOM].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_BOTTOM].line_width = 1.0;
    m->l[SLOPE_AXIS2D_BOTTOM].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_LEFT].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_LEFT].line_width = 1.0;
    m->l[SLOPE_AXIS2D_LEFT].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_TOP].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_TOP].line_width = 1.0;
    m->l[SLOPE_AXIS2D_TOP].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_RIGHT].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_RIGHT].line_width = 1.0;
    m->l[SLOPE_AXIS2D_RIGHT].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_X].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_X].line_width = 1.0;
    m->l[SLOPE_AXIS2D_X].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_Y].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_Y].line_width = 1.0;
    m->l[SLOPE_AXIS2D_Y].options = SLOPE_AXIS2D_DRAW_TICKS;

    slope_axis2d_update_scale (axis);
}


static void
axis2d_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->dispose (object);
}


static void
axis2d_finalize(GObject *object)
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
    SlopeItemPrivate *item_p;
    SlopeTree *iter;
    SlopeItem *item;
    double plt_x_min, plt_x_max;
    double plt_y_min, plt_y_max;

    g_return_if_fail (SLOPE_IS_AXIS2D (self));

    m = SLOPE_AXIS2D_GET_PRIVATE (self);
    item_p = SLOPE_ITEM_GET_PRIVATE (self);

    /* init values for the 'no child' case */
    m->dat_x_min = 0.0;
    m->dat_x_max = 1.0;
    m->dat_y_min = 0.0;
    m->dat_y_max = 1.0;

    if (!(iter = SLOPE_TREE (item_p)->first)) {
        /* no subitems */
        return;
    }

    /* init axis extents with the first plot's extents */
    item = SLOPE_ITEM_PRIVATE (iter)->publ_obj;
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
        item = SLOPE_ITEM_PRIVATE (iter)->publ_obj;
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
}

/* slope/axis2d.c */
