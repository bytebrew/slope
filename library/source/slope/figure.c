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

#define SlopeFigure_RoundedRect   (1U)


typedef struct _SlopeFigurePrivate SlopeFigurePrivate;
#define SLOPE_FIGURE_PRIVATE(Addr) ((SlopeFigurePrivate*) (Addr))

struct _SlopeFigurePrivate
{
    SlopeRGB bg_fill_color;
    SlopeRGB bg_stroke_color;
    double bg_stroke_width;
    GList *item_list;
    guint64 options;
};


G_DEFINE_TYPE_WITH_PRIVATE(SlopeFigure, slope_figure, G_TYPE_OBJECT)


#define SLOPE_FIGURE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_FIGURE, SlopeFigurePrivate))


/* local decls */
static void slope_figure_finalize (GObject *self);
static void slope_figure_dispose (GObject *self);
static void base_figure_draw (SlopeFigure *self, cairo_t *cr, SlopeRect *rect);


static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_figure_dispose;
    gobject_class->finalize = slope_figure_finalize;

    klass->draw = base_figure_draw;

}

static void
slope_figure_init (SlopeFigure *self)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    m->bg_fill_color = SlopeRGB_White;
    m->bg_stroke_color = SlopeRGB_Black;
    m->options = SlopeFigure_RoundedRect;
    m->bg_stroke_width = 2.0;
    m->item_list = NULL;
}


static void
slope_figure_dispose (GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    GList *iter = m->item_list;

    while (iter != NULL) {
        SlopeItem *item = (SlopeItem *) iter->data;
        g_object_unref (G_OBJECT (item));
        iter = iter->next;
    }
    g_list_free (m->item_list);

    G_OBJECT_CLASS (slope_figure_parent_class)->dispose (object);
}


static void
slope_figure_finalize(GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    // TODO

    G_OBJECT_CLASS (slope_figure_parent_class)->finalize (object);
}


SlopeFigure*
slope_figure_new (void)
{
    return SLOPE_FIGURE (g_object_new (SLOPE_TYPE_FIGURE, NULL));
}


static void
base_figure_draw_rect (SlopeFigure *self, cairo_t *cr, SlopeRect *rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (slope_enabled(m->options, SlopeFigure_RoundedRect)) {
        rect->x += 10;
        rect->y += 10;
        rect->width -= 20;
        rect->height -= 20;
        slope_draw_round_rect (cr, rect, 10);
    } else {
        slope_draw_rect (cr, rect);
    }

    slope_draw (cr, m->bg_stroke_width,
                m->bg_fill_color, m->bg_stroke_color);
}


static void
base_figure_draw (SlopeFigure *self, cairo_t *cr, SlopeRect *rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    GList *iter = m->item_list;

    if (slope_rgb_is_visible(m->bg_fill_color) ||
            slope_rgb_is_visible(m->bg_stroke_color)) {
        base_figure_draw_rect (self, cr, rect);
    }

    while (iter != NULL) {
        SlopeItem *item = (SlopeItem *) iter->data;
        /* TPDP: draw items */
        iter = iter->next;
    }
}


void
slope_figure_draw (SlopeFigure *self, cairo_t *cr, SlopeRect *rect)
{
    SlopeRect Mrect;
    g_return_if_fail (SLOPE_IS_FIGURE (self));
    /* copy rect to avoid changing users data */
    Mrect = *rect;
    SLOPE_FIGURE_GET_CLASS (self)->draw(self, cr, &Mrect);
}


int
slope_figure_save (SlopeFigure *self, const gchar *file_name,
                   int width, int height, const gchar *format)
{
    cairo_surface_t *surf;
    cairo_t *cr;
    SlopeRect rect;

    g_return_val_if_fail (SLOPE_IS_FIGURE (self), -1);
    g_return_val_if_fail (file_name, -1);
    g_return_val_if_fail (width > 0 && height > 0, -1);
    g_return_val_if_fail (format, -1);

    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) width;
    rect.height = (double) height;

    if (g_strcmp0(format, "png") == 0) {
        surf = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
        cr = cairo_create (surf);
        slope_figure_draw (self, cr, &rect);
        cairo_surface_write_to_png (surf, file_name);
    } else {
        return -2;
    }

    cairo_surface_destroy (surf);
    cairo_destroy (cr);
    return 0;
}

/* slope/figure.c */
