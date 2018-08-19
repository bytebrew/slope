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
#include "slope/item.h"

typedef struct _SlopeGridPrivate SlopeGridPrivate;
#define SLOPE_GRID_PRIVATE(Addr) ((SlopeGridPrivate*) (Addr))

struct _SlopeGridPrivate
{
    int n_rows;
    int n_cols;
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeGrid, slope_grid, SLOPE_TYPE_ITEM)
#define SLOPE_GRID_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_GRID, SlopeGridPrivate))


/* local decls */
static void grid_finalize(GObject *self);
static void grid_dispose (GObject *self);
static void grid_draw_tree (SlopeItem *self, SlopeItemDC *dc);


static void
slope_grid_class_init (SlopeGridClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = grid_dispose;
    gobject_class->finalize = grid_finalize;

    item_class->draw_tree = grid_draw_tree;
}


static void
slope_grid_init (SlopeGrid *grid)
{
    SlopeGridPrivate *m = SLOPE_GRID_GET_PRIVATE (grid);

    m->n_rows = 1;
    m->n_cols = 1;
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
slope_grid_update_layout (SlopeGrid *self)
{
    SlopeGridPrivate *m;
    SlopeTree *node;

    g_return_if_fail (SLOPE_IS_GRID (self));
    m = SLOPE_GRID_GET_PRIVATE (self);
    node = slope_item_get_fisrt_child (SLOPE_ITEM (self));

    m->n_rows = m->n_cols = 0;

    while (node != NULL) {

        if ((node->x + node->width) > m->n_cols) {
            m->n_cols = node->x + node->width;
        }
        if ((node->y + node->height) > m->n_rows) {
            m->n_rows = node->y + node->height;
        }

        node = node->next;
    }
}


void
slope_grid_emplace (SlopeGrid *self, SlopeItem *child,
                    int x, int y, int width, int height)
{
    SlopeTree *node;

    g_return_if_fail (SLOPE_IS_GRID (self));
    g_return_if_fail (SLOPE_IS_ITEM (child));

    /* set child layout information */
    node = slope_item_get_tree_node (child);
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;

    /* Add new child to this item's tree */
    slope_item_append (SLOPE_ITEM (self), child);

    /* make sure the layout boundaries are correct */
    slope_grid_update_layout (self);
}


static void
grid_draw_tree (SlopeItem *self, SlopeItemDC *dc)
{
    SlopeGridPrivate *m = SLOPE_GRID_GET_PRIVATE (self);
    SlopeRect rect, orig_rect = dc->rect;
    SlopeTree *node = slope_item_get_fisrt_child (self);
    double cell_width, cell_height;

    cell_width = orig_rect.width / m->n_cols;
    cell_height = orig_rect.height / m->n_rows;

    while (node != NULL) {
        SlopeItem *item = slope_item_from_tree_node (node);

        rect.x = node->x * cell_width;
        rect.y = node->y * cell_height;
        rect.width = node->width * cell_width;
        rect.height = node->height * cell_height;

        dc->rect = rect;
        SLOPE_ITEM_GET_CLASS (item)->draw_tree (item, dc);
        node = node->next;
    }

    /* Give the DC back with the original rect */
    dc->rect = orig_rect;
}

/* slope/grid.c */
