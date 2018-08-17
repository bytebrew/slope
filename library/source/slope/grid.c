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
    SlopeRGBA lines_color;
    double lines_width;
};

G_DEFINE_TYPE_WITH_PRIVATE (SlopeGrid, slope_grid, SLOPE_TYPE_ITEM)
#define SLOPE_GRID_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_GRID, SlopeGridPrivate))

/* local decls */
static void slope_grid_finalize(GObject *self);
static void slope_grid_dispose (GObject *self);
static void grid_draw (SlopeItem *self, const SlopeItemDC *dc);

static void
slope_grid_class_init (SlopeGridClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = slope_grid_dispose;
    gobject_class->finalize = slope_grid_finalize;

    item_class->draw = grid_draw;
}


static void
slope_grid_init (SlopeGrid *grid)
{
    SlopeGridPrivate *m = SLOPE_GRID_GET_PRIVATE (grid);

    m->lines_color = SLOPE_BLACK;
    m->lines_width = 1.0;
}


static void
slope_grid_dispose (GObject *object)
{
    // TODO
    G_OBJECT_CLASS (slope_grid_parent_class)->dispose (object);
}


static void
slope_grid_finalize(GObject *object)
{
    // TODO
    G_OBJECT_CLASS (slope_grid_parent_class)->finalize (object);
}


SlopeItem*
slope_grid_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_GRID, NULL));
}


static void
grid_draw (SlopeItem *self, const SlopeItemDC *dc)
{
    SlopeGridPrivate *m = SLOPE_GRID_GET_PRIVATE (self);
    SlopeRect rec = *dc->parent_rect;

    rec.x += 10;
    rec.y += 10;
    rec.width -= 20;
    rec.height -= 20;

    /* just to show something */
    /* TODO: add the grid logic */
    slope_cairo_rect (dc->cr, &rec);
    slope_cairo_set_rgba(dc->cr, m->lines_color);
    cairo_set_line_width (dc->cr, m->lines_width);
    cairo_set_antialias (dc->cr, CAIRO_ANTIALIAS_NONE);
    cairo_stroke (dc->cr);
}


void
slope_grid_emplace (SlopeGrid *self, SlopeItem *child)
{
    // TODO
}

/* slope/view.c */
