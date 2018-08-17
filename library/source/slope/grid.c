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

#include "slope/grid.h"

typedef struct _SlopeGridPrivate SlopeGridPrivate;
#define SLOPE_GRID_PRIVATE(Addr) ((SlopeGridPrivate*) (Addr))

struct _SlopeGridPrivate
{
    double user_x_min, user_x_max;
    double user_y_min, user_y_max;

    double grid_x_min, grid_x_max;
    double grid_y_min, grid_y_max;
};

G_DEFINE_TYPE_WITH_PRIVATE (SlopeGrid, slope_grid, SLOPE_TYPE_ITEM)
#define SLOPE_GRID_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_GRID, SlopeGridPrivate))

/* local decls */
static void slope_grid_finalize(GObject *self);
static void slope_grid_dispose (GObject *self);


static void
slope_grid_class_init (SlopeGridClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_grid_dispose;
    gobject_class->finalize = slope_grid_finalize;
}


static void
slope_grid_init (SlopeGrid *grid)
{

}


static void
slope_grid_dispose (GObject *object)
{

  G_OBJECT_CLASS (slope_grid_parent_class)->dispose (object);
}


static void
slope_grid_finalize(GObject *object)
{

    G_OBJECT_CLASS (slope_grid_parent_class)->finalize (object);
}


SlopeItem*
slope_grid_new (void)
{
 return SLOPE_ITEM (g_object_new (SLOPE_TYPE_GRID, NULL));
}

/* slope/view.c */
