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

#include "slope/figure_p.h"
#include "slope/item_p.h"
#include <cairo/cairo-svg.h>
#include <cairo/cairo-ps.h>


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


/* properties */
enum {
  PROP_0,
  PROP_BG_FILL_COLOR,
  PROP_BG_STROKE_COLOR,
  PROP_BG_STROKE_WIDTH,
  PROP_LAST
};
static GParamSpec *figure_props[PROP_LAST] = { NULL };


/* local decls */
static void figure_finalize (GObject *self);
static void figure_dispose (GObject *self);
static void figure_draw (SlopeFigure *self, cairo_t *cr, int width, int height);
static void figure_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void figure_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void figure_set_root_item (SlopeFigure *self, SlopeItem *item);


static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = figure_dispose;
    gobject_class->finalize = figure_finalize;
    gobject_class->set_property = figure_set_property;
    gobject_class->get_property = figure_get_property;

    klass->draw = figure_draw;
    klass->set_root_item = figure_set_root_item;

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

    m->bg_fill_color = SLOPE_WHITE;
    m->bg_stroke_color = SLOPE_MAROON;
    m->options = DrawRect | RoundedRect | DrawTitle;
    m->bg_stroke_width = 1.5;
    m->root_item = NULL;
    m->text = slope_text_new ("Monospace 9");
    m->title = g_strdup("Slope");
    m->title_color = SLOPE_BLACK;
}


static void
figure_set_property (GObject *object, guint prop_id,
                     const GValue *value, GParamSpec *pspec)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE(object);
    SLOPE_UNUSED (pspec);

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
figure_get_property (GObject *object, guint prop_id,
                     GValue *value, GParamSpec *pspec)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE(object);
    SLOPE_UNUSED (pspec);

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


static void
figure_dispose (GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (m->root_item != NULL) {
        slope_item_destroy_tree (m->root_item);
    }

    slope_text_delete (m->text);
    slope_figure_set_title (self, NULL);

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
figure_draw_rect (SlopeFigure *self, cairo_t *cr, SlopeRect *rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (!slope_enabled(m->options, DrawRect) ||
            (!slope_rgba_is_visible(m->bg_fill_color) &&
             !slope_rgba_is_visible(m->bg_stroke_color))) {
        return;
    }

    if (slope_enabled(m->options, RoundedRect)) {
        /* translate cr to account for the area loss */
        rect->width -= 20;
        rect->height -= 20;
        cairo_translate (cr, 10.0, 10.0);
        slope_cairo_round_rect (cr, rect, 10);
    } else {
        slope_cairo_rect (cr, rect);
    }

    slope_cairo_draw (
          cr, m->bg_stroke_width,
          m->bg_fill_color, m->bg_stroke_color);
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
figure_draw_title (SlopeFigure *self,
                   cairo_t *cr, const SlopeRect *rect)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    SlopeRect ink, logical;

    if (!slope_enabled(m->options, DrawTitle) ||
            (m->title == NULL) ||
            !slope_rgba_is_visible(m->title_color) ||
            (m->title_color == m->bg_fill_color)) {
        return;
    }

    slope_text_set (m->text, m->title);
    slope_text_get_extents (m->text, &ink, &logical);
    slope_cairo_set_rgba (cr, m->title_color);
    cairo_move_to (cr, (rect->width - logical.width) / 2.0, 10.0);
    slope_text_show (m->text);
}


static void
figure_draw (SlopeFigure *self, cairo_t *cr, int width, int height)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);
    SlopeRect rect;

    /* Save the cairo context's state so we can
     * return it unchanged */
    cairo_save (cr);
    slope_text_init (m->text, cr);

    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) width;
    rect.height = (double) height;

    figure_draw_rect (self, cr, &rect);
    figure_draw_items (self, cr, &rect);
    figure_draw_title (self, cr, &rect);

    /* Give the cairo context back to the user in
     * the same state in which we received it */
    cairo_restore (cr);
}


void /* public draw() function*/
slope_figure_draw (SlopeFigure *self, cairo_t *cr, int width, int height)
{
    g_return_if_fail (SLOPE_IS_FIGURE (self));
    SLOPE_FIGURE_GET_CLASS (self)->draw (self, cr, width, height);
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
        slope_figure_draw (self, cr, width, height);
        cairo_surface_write_to_png (surf, file_name);
    } else if (g_strcmp0(format, "svg") == 0) {
        surf = cairo_svg_surface_create(file_name, width, height);
        cr = cairo_create (surf);
        slope_figure_draw (self, cr, width, height);
    } else {
        return -2;
    }

    cairo_surface_destroy (surf);
    cairo_destroy (cr);
    return 0;
}


void /* public set_title() */
slope_figure_set_title (SlopeFigure *self, const gchar *title)
{
    SlopeFigurePrivate *m;

    g_return_if_fail(SLOPE_IS_FIGURE(self));
    m = SLOPE_FIGURE_GET_PRIVATE (self);

    if (m->title != NULL) {
        g_free (m->title);
        m->title = NULL;
    }

    if (title != NULL) {
        m->title = g_strdup(title);
    }
}


const gchar* /* public get_title() */
slope_figure_get_title (SlopeFigure *self)
{
    g_return_val_if_fail (SLOPE_IS_FIGURE (self), NULL);
    return SLOPE_FIGURE_GET_PRIVATE (self)->title;
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
