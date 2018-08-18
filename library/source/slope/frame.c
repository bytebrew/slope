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

#include "slope/frame.h"
#include "slope/item_p.h"


typedef struct _SlopeFramePrivate SlopeFramePrivate;
#define SLOPE_FRAME_PRIVATE(Addr) ((SlopeFramePrivate*) (Addr))


struct _SlopeFramePrivate
{
    SlopeRGBA bg_fill_color;
    SlopeRGBA bg_stroke_color;
    double bg_stroke_width;
    int margin;
    SlopeRGBA title_color;
    gchar *title;
    guint32 options;
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeFrame, slope_frame, SLOPE_TYPE_ITEM)
#define SLOPE_FRAME_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_FRAME, SlopeFramePrivate))


/* properties */
enum {
  PROP_0,
  PROP_BG_FILL_COLOR,
  PROP_BG_STROKE_COLOR,
  PROP_BG_STROKE_WIDTH,
  PROP_LAST
};
static GParamSpec *frame_props[PROP_LAST] = { NULL };


/* local decls */
static void frame_finalize (GObject *self);
static void frame_dispose (GObject *self);
static void frame_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void frame_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void frame_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void frame_draw_tree (SlopeItem *self, SlopeItemDC *dc);

static void
slope_frame_class_init (SlopeFrameClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = frame_dispose;
    gobject_class->finalize = frame_finalize;
    gobject_class->set_property = frame_set_property;
    gobject_class->get_property = frame_get_property;

    item_class->draw_self = frame_draw_self;
    item_class->draw_tree = frame_draw_tree;

    frame_props[PROP_BG_FILL_COLOR] =
          g_param_spec_uint ("bg-fill-color",
                             "Background fill color",
                             "Specify the background fill color",
                             SLOPE_COLOR_NULL, SLOPE_WHITE, SLOPE_WHITE,
                             G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    frame_props[PROP_BG_STROKE_COLOR] =
          g_param_spec_uint ("bg-stroke-color",
                             "Background stroke color",
                             "Specify the background stroke color",
                             SLOPE_COLOR_NULL, SLOPE_WHITE, SLOPE_COLOR_NULL,
                             G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    frame_props[PROP_BG_STROKE_WIDTH] =
          g_param_spec_double ("bg-stroke-width",
                               "Background stroke width",
                               "Specify the background stroke width",
                               0.0, 8.0, 2.0,
                               G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);

    g_object_class_install_properties (gobject_class, PROP_LAST, frame_props);
}


static void
slope_frame_init (SlopeFrame *axis)
{
    SlopeFramePrivate *m = SLOPE_FRAME_GET_PRIVATE (axis);

    /* Visible frame initialization */
    m->bg_fill_color = SLOPE_WHITE;
    m->bg_stroke_color = SLOPE_MAROON;
    m->options = SLOPE_FRAME_DRAW_RECT | SLOPE_FRAME_DRAW_TITLE | SLOPE_FRAME_ROUND_RECT;
    m->bg_stroke_width = 2.0;
    m->title = g_strdup("Slope");
    m->title_color = SLOPE_BLACK;
    m->margin = 4;
}


static void
frame_dispose (GObject *object)
{
    slope_frame_set_title (SLOPE_FRAME (object), NULL);

    G_OBJECT_CLASS (slope_frame_parent_class)->dispose (object);
}


static void
frame_finalize(GObject *object)
{
    G_OBJECT_CLASS (slope_frame_parent_class)->finalize (object);
}


static void
frame_set_property (GObject *object, guint prop_id,
                     const GValue *value, GParamSpec *pspec)
{
    SlopeFramePrivate *m = SLOPE_FRAME_GET_PRIVATE(object);
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
frame_get_property (GObject *object, guint prop_id,
                     GValue *value, GParamSpec *pspec)
{
    SlopeFramePrivate *m = SLOPE_FRAME_GET_PRIVATE(object);
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


SlopeItem*
slope_frame_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_FRAME, NULL));
}


static void
frame_draw_rect (SlopeFrame *self, const SlopeItemDC *dc)
{
    SlopeFramePrivate *m = SLOPE_FRAME_GET_PRIVATE (self);
    SlopeRect r = dc->rect;

    if (!slope_enabled(m->options, SLOPE_FRAME_DRAW_RECT) ||
            (!slope_rgba_is_visible(m->bg_fill_color) &&
             !slope_rgba_is_visible(m->bg_stroke_color))) {
        return;
    }

    if (slope_rgba_is_visible(m->bg_stroke_color)) {
        /* avoid rounding stroke from being clipped away */
        r.x += m->bg_stroke_width / 2;
        r.y += m->bg_stroke_width / 2;
        r.width -= 2 * m->bg_stroke_width;
        r.height -= 2 * m->bg_stroke_width;
    }

    if (slope_enabled(m->options, SLOPE_FRAME_ROUND_RECT)) {
        slope_cairo_round_rect (dc->cr, &r, 10);
    } else {
        slope_cairo_rect (dc->cr, &r);
    }

    slope_cairo_draw (
          dc->cr, m->bg_stroke_width,
          m->bg_fill_color, m->bg_stroke_color);
}


static void
frame_draw_title (SlopeFrame *self, const SlopeItemDC *dc)
{
    SlopeFramePrivate *m = SLOPE_FRAME_GET_PRIVATE (self);
    SlopeRect ink, logical;
    cairo_t *cr = dc->cr;

    if (!slope_enabled(m->options, SLOPE_FRAME_DRAW_TITLE) ||
            (m->title == NULL) ||
            !slope_rgba_is_visible(m->title_color) ||
            (m->title_color == m->bg_fill_color)) {
        return;
    }

    slope_text_set (dc->text, m->title);
    slope_text_get_extents (dc->text, &ink, &logical);
    slope_cairo_set_rgba (cr, m->title_color);
    cairo_move_to (cr, (dc->rect.width - logical.width) / 2.0, 10.0);
    slope_text_show (dc->text);
}


static void
frame_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    frame_draw_rect (SLOPE_FRAME (self), dc);
    frame_draw_title (SLOPE_FRAME (self), dc);
}


void /* public set_title() */
slope_frame_set_title (SlopeFrame *self, const gchar *title)
{
    SlopeFramePrivate *m;

    g_return_if_fail (SLOPE_IS_FRAME (self));
    m = SLOPE_FRAME_GET_PRIVATE (self);

    if (m->title != NULL) {
        g_free (m->title);
        m->title = NULL;
    }

    if (title != NULL) {
        m->title = g_strdup(title);
    }
}


const gchar* /* public get_title() */
slope_frame_get_title (SlopeFrame *self)
{
    g_return_val_if_fail (SLOPE_IS_FRAME (self), NULL);
    return SLOPE_FRAME_GET_PRIVATE (self)->title;
}


static void
frame_draw_tree (SlopeItem *self, SlopeItemDC *dc)
{
    SlopeFramePrivate *m = SLOPE_FRAME_GET_PRIVATE (self);
    int margin = m->margin;
    SlopeRect orig_rect = dc->rect;

    cairo_save (dc->cr);

    /* If stuff like margins are to be applyed, we need to
     * adjust the rectangle to which subitems will have access to */
    if (margin > 0) {
        cairo_translate (dc->cr, m->margin, m->margin);
        dc->rect.width -= 2 * margin;
        dc->rect.height -= 2 * margin;
    }

    slope_cairo_rect (dc->cr, &dc->rect);
    cairo_clip (dc->cr);

    SLOPE_ITEM_CLASS (slope_frame_parent_class)->draw_tree (self, dc);

    cairo_restore (dc->cr);
    dc->rect = orig_rect;
}

/* slope/frame.c */
