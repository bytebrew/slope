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

#include "slope/figure.h"
#include "slope/item.h"
#include <cairo/cairo-svg.h>
#include <cairo/cairo-ps.h>

typedef struct _SlopeFigurePrivate SlopeFigurePrivate;
#define SLOPE_FIGURE_PRIVATE(Addr) ((SlopeFigurePrivate*) (Addr))

struct _SlopeFigurePrivate
{
    SlopeItem *root_item;
    guint64 options;
    SlopeText *text;
};


#define SLOPE_FIGURE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_FIGURE, SlopeFigurePrivate))


G_DEFINE_TYPE_WITH_PRIVATE(SlopeFigure, slope_figure, G_TYPE_OBJECT)


/* signals */
enum {
    SIG_ADD,
    SIG_REMOVE,
    SIG_CHANGED,
    SIG_NUMERR,
    SIG_LAST
};
static guint figure_signals[SIG_LAST] = { 0 };


/* local decls */
static void figure_finalize (GObject *self);
static void figure_dispose (GObject *self);
static void figure_draw (SlopeFigure *self, cairo_t *cr, int x, int y, int width, int height);
static void figure_set_root_item (SlopeFigure *self, SlopeItem *item);


static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = figure_dispose;
    gobject_class->finalize = figure_finalize;

    klass->draw = figure_draw;
    klass->set_root_item = figure_set_root_item;

    figure_signals[SIG_ADD] =
        g_signal_new ("add",
                      G_OBJECT_CLASS_TYPE (gobject_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (SlopeFigureClass, set_root_item),
                      NULL, NULL,
                      NULL,
                      G_TYPE_NONE, 0);

    figure_signals[SIG_CHANGED] =
        g_signal_new ("changed",
                      G_OBJECT_CLASS_TYPE (gobject_class),
                      G_SIGNAL_RUN_FIRST,
                      0,
                      NULL, NULL,
                      NULL,
                      G_TYPE_NONE,
                      0);


    figure_signals[SIG_NUMERR] =
        g_signal_new ("numeric-error",
                      G_OBJECT_CLASS_TYPE (gobject_class),
                      G_SIGNAL_RUN_FIRST,
                      0,
                      NULL, NULL,
                      NULL,
                      G_TYPE_NONE,
                      0);
}


static void
slope_figure_init (SlopeFigure *self)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    m->options = 0UL;
    m->root_item = NULL;
    m->text = slope_text_new ("Monospace 9");
}


static void
figure_dispose (GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (m->root_item != NULL) {
        slope_item_destroy_tree (m->root_item);
    }

    slope_text_delete (m->text);

    G_OBJECT_CLASS (slope_figure_parent_class)->dispose (object);
}


static void
figure_finalize (GObject *object)
{
    G_OBJECT_CLASS (slope_figure_parent_class)->finalize (object);
}


SlopeFigure* /* public new() function*/
slope_figure_new (void)
{
    return SLOPE_FIGURE (g_object_new (SLOPE_TYPE_FIGURE, NULL));
}


static void
figure_draw_items (SlopeFigure *self,
                   cairo_t *cr, const SlopeRect *rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    SlopeItemDC dc;

    if (m->root_item == NULL) {
        return;
    }

    dc.cr = cr;
    dc.figure = self;
    dc.parent = NULL;
    dc.rect = *rect;
    dc.text = m->text;

    SLOPE_ITEM_GET_CLASS (m->root_item)->draw_tree (m->root_item, &dc);
}


static void
figure_draw (SlopeFigure *self, cairo_t *cr,
             int x, int y, int width, int height)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    SlopeRect rect;

    /* Save the cairo context's state */
    cairo_save (cr);
    /* let's items think we are at the origin */
    cairo_translate (cr, x, y);
    /* don't let them draw outside */
    cairo_rectangle (cr, 0.0, 0.0, width, height);
    cairo_clip (cr);

    /* Initialize text rendering stuff */
    slope_text_init (m->text, cr);

    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) width;
    rect.height = (double) height;

    figure_draw_items (self, cr, &rect);

    /* Give the cairo context back to the user in
     * the same state in which we received it */
    cairo_restore (cr);
}


void /* public draw() function*/
slope_figure_draw (SlopeFigure *self, cairo_t *cr,
                   int x, int y, int width, int height)
{
    g_return_if_fail (SLOPE_IS_FIGURE (self));
    SLOPE_FIGURE_GET_CLASS (self)->draw (self, cr, x, y, width, height);
}


int /* public save() function*/
slope_figure_save (SlopeFigure *self, const gchar *file_name,
                   int width, int height, const gchar *format)
{
    cairo_surface_t *surf;
    cairo_t *cr;

    g_return_val_if_fail (SLOPE_IS_FIGURE (self), -1);
    g_return_val_if_fail (file_name, -1);
    g_return_val_if_fail (width > 0 && height > 0, -1);
    g_return_val_if_fail (format, -1);

    if (g_strcmp0(format, "png") == 0) {
        surf = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
        cr = cairo_create (surf);
        slope_figure_draw (self, cr, 0, 0, width, height);
        cairo_surface_write_to_png (surf, file_name);
    } else if (g_strcmp0(format, "svg") == 0) {
        surf = cairo_svg_surface_create(file_name, width, height);
        cr = cairo_create (surf);
        slope_figure_draw (self, cr, 0, 0, width, height);
    } else {
        return -2;
    }

    cairo_surface_destroy (surf);
    cairo_destroy (cr);
    return 0;
}


static void
figure_set_root_item (SlopeFigure *self, SlopeItem *item)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    /* Clear any current content */
    if (m->root_item != NULL) {
        slope_item_destroy_tree (m->root_item);
    }

    m->root_item = item;
    SLOPE_ITEM_GET_CLASS (item)->attached (item, NULL);

    g_signal_emit_by_name (self, "changed");
}


void /* public set_root_item() */
slope_figure_set_root_item (SlopeFigure *self, SlopeItem *item) {
    g_return_if_fail (SLOPE_IS_FIGURE (self));
    g_return_if_fail (SLOPE_IS_ITEM (item));
    SLOPE_FIGURE_GET_CLASS (self)->set_root_item (self, item);
}

/* slope/figure.c */
