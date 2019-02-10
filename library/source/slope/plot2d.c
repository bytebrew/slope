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

#include "slope/plot2d.h"
#include "slope/item.h"

typedef struct _SlopePlot2DPrivate SlopePlot2DPrivate;
#define SLOPE_PLOT2D_PRIVATE(Addr) ((SlopePlot2DPrivate*) (Addr))


struct _SlopePlot2DPrivate
{
    char _dummy_padding;
};


#define SLOPE_PLOT2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_PLOT2D, SlopePlot2DPrivate))


G_DEFINE_TYPE_WITH_PRIVATE (SlopePlot2D, slope_plot2d, SLOPE_TYPE_ITEM)


/* local decls */
static void plot2d_finalize (GObject *self);
static void plot2d_dispose (GObject *self);
static void plot2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void plot2d_get_data_rect (SlopePlot2D *self, SlopeRect *rect);


static void
slope_plot2d_class_init (SlopePlot2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = plot2d_dispose;
    gobject_class->finalize = plot2d_finalize;

    item_class->draw_self = plot2d_draw_self;

    klass->get_data_rect = plot2d_get_data_rect;
}


static void
slope_plot2d_init (SlopePlot2D *plot)
{
    SlopePlot2DPrivate *m = SLOPE_PLOT2D_GET_PRIVATE (plot);
}


static void
plot2d_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_plot2d_parent_class)->dispose (object);
}


static void
plot2d_finalize(GObject *object)
{
    G_OBJECT_CLASS (slope_plot2d_parent_class)->finalize (object);
}


static void
plot2d_get_data_rect (SlopePlot2D *self, SlopeRect *rect)
{
    SLOPE_UNUSED(self);
    rect->x = 0.0;
    rect->y = 0.0;
    rect->width = 1.0;
    rect->height = 1.0;
}


void
slope_plot2d_get_data_rect (SlopePlot2D *self, SlopeRect *rect)
{
    g_assert (SLOPE_IS_PLOT2D (self));
    g_assert (rect);
    SLOPE_PLOT2D_GET_CLASS (self)->get_data_rect (self, rect);
}


void
slope_plot2d_get_data_extents (SlopePlot2D *self,
                               double *x_min, double *x_max,
                               double *y_min, double *y_max)
{
    SlopeRect rect;

    g_assert (SLOPE_IS_PLOT2D (self));
    g_assert (x_min && x_max);
    g_assert (y_min && y_max);

    slope_plot2d_get_data_rect(self, &rect);

    *x_min = rect.x;
    *y_min = rect.y;
    *x_max = rect.x + rect.width;
    *y_max = rect.y + rect.height;
}


static void
plot2d_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    SLOPE_ITEM_CLASS (slope_plot2d_parent_class)->draw_self (self, dc);
}


/* slope/plot2d.c */
