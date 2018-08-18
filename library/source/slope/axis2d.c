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
    guint32 options;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
};


struct _SlopeAxis2DPrivate
{
    AxisLine lines[6];
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeAxis2D, slope_axis2d, SLOPE_TYPE_ITEM)
#define SLOPE_AXIS2D_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))

/* local decls */
static void axis2d_finalize (GObject *self);
static void axis2d_dispose (GObject *self);
static void axis2d_draw (SlopeItem *self, const SlopeItemDC *dc);


static void
slope_axis2d_class_init (SlopeAxis2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = axis2d_dispose;
    gobject_class->finalize = axis2d_finalize;

    item_class->draw = axis2d_draw;
}


static void
slope_axis2d_init (SlopeAxis2D *axis)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (axis);

    m->lines[SLOPE_AXIS2D_BOTTOM].line_color = SLOPE_BLACK;
    m->lines[SLOPE_AXIS2D_BOTTOM].line_width = 1.0;
    m->lines[SLOPE_AXIS2D_BOTTOM].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->lines[SLOPE_AXIS2D_LEFT].line_color = SLOPE_BLACK;
    m->lines[SLOPE_AXIS2D_LEFT].line_width = 1.0;
    m->lines[SLOPE_AXIS2D_LEFT].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->lines[SLOPE_AXIS2D_TOP].line_color = SLOPE_BLACK;
    m->lines[SLOPE_AXIS2D_TOP].line_width = 1.0;
    m->lines[SLOPE_AXIS2D_TOP].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->lines[SLOPE_AXIS2D_RIGHT].line_color = SLOPE_BLACK;
    m->lines[SLOPE_AXIS2D_RIGHT].line_width = 1.0;
    m->lines[SLOPE_AXIS2D_RIGHT].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->lines[SLOPE_AXIS2D_X].line_color = SLOPE_BLACK;
    m->lines[SLOPE_AXIS2D_X].line_width = 1.0;
    m->lines[SLOPE_AXIS2D_X].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->lines[SLOPE_AXIS2D_Y].line_color = SLOPE_BLACK;
    m->lines[SLOPE_AXIS2D_Y].line_width = 1.0;
    m->lines[SLOPE_AXIS2D_Y].options = SLOPE_AXIS2D_DRAW_TICKS;
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
axis2d_draw (SlopeItem *self, const SlopeItemDC *dc)
{
    /* TODO: */
    SLOPE_UNUSED(self);
    SLOPE_UNUSED(dc);
}

/* slope/axis2d.c */
