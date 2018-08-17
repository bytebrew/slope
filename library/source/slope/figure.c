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
#include "slope/item_p.h"
#include "slope/tree.h"
#include <gtk/gtk.h>
#define RoundedRect   (1U)


typedef struct _SlopeFigurePrivate SlopeFigurePrivate;
#define SLOPE_FIGURE_PRIVATE(Addr) ((SlopeFigurePrivate*) (Addr))

struct _SlopeFigurePrivate
{
    SlopeRGBA bg_fill_color;
    SlopeRGBA bg_stroke_color;
    double bg_stroke_width;
    SlopeTree *item_tree;
    guint64 options;
    SlopeText *text;

    SlopeRGBA title_color;
    gchar *title;
};


G_DEFINE_TYPE_WITH_PRIVATE(SlopeFigure, slope_figure, G_TYPE_OBJECT)


/* signals */
enum {
    SIG_ADD,
    SIG_REMOVE,
    SIG_CHANGED,
    SIG_LAST
};
static guint figure_signals[SIG_LAST] = { 0 };


/* properties */
enum {
  PROP_0,
  PROP_BG_FILL_COLOR,
  PROP_BG_STROKE_COLOR,
  PROP_BG_STROKE_WIDTH,
  PROP_LAST
};
static GParamSpec *figure_props[PROP_LAST] = { NULL };


#define SLOPE_FIGURE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_FIGURE, SlopeFigurePrivate))


/* local decls */
static void slope_figure_finalize (GObject *self);
static void slope_figure_dispose (GObject *self);
static void base_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);
static void slope_figure_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void slope_figure_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);


static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_figure_dispose;
    gobject_class->finalize = slope_figure_finalize;
    gobject_class->set_property = slope_figure_set_property;
    gobject_class->get_property = slope_figure_get_property;

    klass->draw = base_figure_draw;

    figure_props[PROP_BG_FILL_COLOR] =
          g_param_spec_uint ("bg-fill-color",
                             "Background fill color",
                             "Specify the background fill color",
                             SLOPE_COLOR_NULL, SLOPE_WHITE, SLOPE_WHITE,
                             G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    figure_props[PROP_BG_STROKE_COLOR] =
          g_param_spec_uint ("bg-stroke-color",
                             "Background stroke color",
                             "Specify the background stroke color",
                             SLOPE_COLOR_NULL, SLOPE_WHITE, SLOPE_COLOR_NULL,
                             G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    figure_props[PROP_BG_STROKE_WIDTH] =
          g_param_spec_double ("bg-stroke-width",
                               "Background stroke width",
                               "Specify the background stroke width",
                               0.0, 8.0, 2.0,
                               G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    g_object_class_install_properties (gobject_class, PROP_LAST, figure_props);


    figure_signals[SIG_ADD] =
        g_signal_new ("add",
                      G_OBJECT_CLASS_TYPE (gobject_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (SlopeFigureClass, add),
                      NULL, NULL,
                      NULL,
                      G_TYPE_NONE, 0);
}


static void
slope_figure_init (SlopeFigure *self)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    m->bg_fill_color = SLOPE_WHITE;
    m->bg_stroke_color = SLOPE_COLOR_NULL;
    m->options = RoundedRect;
    m->bg_stroke_width = 2.0;
    m->item_tree = NULL;
    m->text = slope_text_new ("Monospace 9");
    m->title = g_strdup("Slope Gtk Plot");
    m->title_color = SLOPE_DARKMAGENTA;
}


static void
slope_figure_set_property (GObject *object, guint prop_id,
                           const GValue *value, GParamSpec *pspec)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE(object);

    switch (prop_id) {
        case PROP_BG_FILL_COLOR:
            m->bg_fill_color = g_value_get_uint(value);
            break;
        case PROP_BG_STROKE_COLOR:
            m->bg_stroke_color = g_value_get_uint(value);
            break;
        case PROP_BG_STROKE_WIDTH:
            m->bg_stroke_width = g_value_get_double(value);
            break;
    }
}


static void
slope_figure_get_property (GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE(object);

    switch (prop_id) {
        case PROP_BG_FILL_COLOR:
            g_value_set_uint(value, m->bg_fill_color);
            break;
        case PROP_BG_STROKE_COLOR:
            g_value_set_uint(value, m->bg_stroke_color);
            break;
        case PROP_BG_STROKE_WIDTH:
            g_value_set_double(value, m->bg_stroke_width);
            break;
    }
}


gpointer item_cleanup (gpointer data, gpointer context)
{
    SLOPE_UNUSED(context);
    g_object_unref (G_OBJECT (data));
    return NULL;
}


static void
slope_figure_dispose (GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    slope_tree_destroy (m->item_tree, item_cleanup);
    slope_text_delete (m->text);

    G_OBJECT_CLASS (slope_figure_parent_class)->dispose (object);
}


static void
slope_figure_finalize (GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    // TODO

    G_OBJECT_CLASS (slope_figure_parent_class)->finalize (object);
}


SlopeFigure* /* public new() function*/
slope_figure_new (void)
{
    return SLOPE_FIGURE (g_object_new (SLOPE_TYPE_FIGURE, NULL));
}


static void
figure_draw_rect (SlopeFigure *self, cairo_t *cr, SlopeRect *rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (slope_enabled(m->options, RoundedRect)) {
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
figure_draw_items (SlopeFigure *self, SlopeTree *node,
                   cairo_t *cr, const SlopeRect *rect)
{
    /* Do this node's drawing */
    draw_item_p ((SlopeItemPrivate *) node, cr, rect);

    /* Recurse to the subtrees */
    node = node->first;
    while (node != NULL) {
        figure_draw_items(self, node, cr, rect);
        node = node->next;
    }
}


static void
base_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *user_rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    SlopeRect rect = *user_rect; /* mutable */

    slope_text_init (m->text, cr);

    if (slope_rgba_is_visible(m->bg_fill_color) ||
            slope_rgba_is_visible(m->bg_stroke_color)) {
        figure_draw_rect (self, cr, &rect);
    }

    if (m->item_tree != NULL) {
        figure_draw_items(self, m->item_tree, cr, &rect);
    }

    /* Draw the title, if is is visible no the current background color */
    if (m->title != NULL &&
            slope_rgba_is_visible(m->title_color) &&
            (m->title_color != m->bg_fill_color)) {
        int width, height;
        slope_text_set (m->text, m->title);
        slope_text_get_size (m->text, &width, &height);
        slope_cairo_set_rgba (cr, m->title_color);
        cairo_move_to (cr, (((int) rect.width) - width) /2.0, 20);
        slope_text_show (m->text);
    }
}


void /* public draw() function*/
slope_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect)
{
    g_return_if_fail (SLOPE_IS_FIGURE (self));
    SLOPE_FIGURE_GET_CLASS (self)->draw(self, cr, rect);
}


int /* public save() function*/
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


void slope_figure_set_title (SlopeFigure *self, const gchar *title)
{
    SlopeFigurePrivate *m;

    g_return_if_fail(SLOPE_IS_FIGURE(self));
    m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (m->title)
        g_free(m->title);

    m->title = NULL;
    if (title)
        m->title = g_strdup(title);
}


const gchar* slope_figure_get_title (SlopeFigure *self)
{
    g_return_val_if_fail(SLOPE_IS_FIGURE(self), NULL);
    return SLOPE_FIGURE_GET_PRIVATE (self)->title;
}

/* slope/figure.c */
