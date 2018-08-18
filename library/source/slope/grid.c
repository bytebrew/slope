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
#include "slope/item_p.h"

typedef struct _SlopeGridPrivate SlopeGridPrivate;
#define SLOPE_GRID_PRIVATE(Addr) ((SlopeGridPrivate*) (Addr))

struct _SlopeGridPrivate
{
    SlopeRGBA lines_color;
    double lines_width;
};

G_DEFINE_TYPE_WITH_PRIVATE (SlopeGrid, slope_grid, SLOPE_TYPE_ITEM)
#define SLOPE_GRID_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_GRID, SlopeGridPrivate))


/* local decls */
static void grid_finalize(GObject *self);
static void grid_dispose (GObject *self);


static void
slope_grid_class_init (SlopeGridClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = grid_dispose;
    gobject_class->finalize = grid_finalize;
}


static void
slope_grid_init (SlopeGrid *grid)
{
    SlopeGridPrivate *m = SLOPE_GRID_GET_PRIVATE (grid);

    m->lines_color = SLOPE_BLACK;
    m->lines_width = 1.0;
}


static void
grid_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_grid_parent_class)->dispose (object);
}


static void
grid_finalize(GObject *object)
{
    G_OBJECT_CLASS (slope_grid_parent_class)->finalize (object);
}


SlopeItem*
slope_grid_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_GRID, NULL));
}


void
slope_grid_emplace (SlopeGrid *self, SlopeItem *child)
{
    g_return_if_fail (SLOPE_IS_GRID (self));
    g_return_if_fail (SLOPE_IS_ITEM (child));

    slope_item_append (SLOPE_ITEM (self), child);
}

/* slope/grid.c */
