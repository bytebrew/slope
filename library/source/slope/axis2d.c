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

#include "slope/axis2d.h"
#include "slope/item_p.h"


#define DrawRect       (1U)
#define RoundedRect    (1U << 1U)
#define DrawTitle      (1U << 2U)


typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))

typedef struct _AxisLine AxisLine;
#define AXIS_LINE(Addr) ((AxisLine *) (Addr))


struct _AxisLine {
    SlopeRGBA line_color;
    double line_width;
    guint32 options;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
};


struct _SlopeAxis2DPrivate
{
    AxisLine l[6];

    SlopeRGBA bg_fill_color;
    SlopeRGBA bg_stroke_color;
    double bg_stroke_width;
    int margin;
    SlopeRGBA title_color;

    gchar *title;

    guint64 options;
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeAxis2D, slope_axis2d, SLOPE_TYPE_ITEM)
#define SLOPE_AXIS2D_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))

/* local decls */
static void axis2d_finalize (GObject *self);
static void axis2d_dispose (GObject *self);
static void axis2d_draw (SlopeItem *self, const SlopeItemDC *dc);


static void
slope_axis2d_class_init (SlopeAxis2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = axis2d_dispose;
    gobject_class->finalize = axis2d_finalize;

    item_class->draw = axis2d_draw;
}


static void
slope_axis2d_init (SlopeAxis2D *axis)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (axis);

    /* metic lines initialization */
    m->l[SLOPE_AXIS2D_BOTTOM].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_BOTTOM].line_width = 1.0;
    m->l[SLOPE_AXIS2D_BOTTOM].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_LEFT].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_LEFT].line_width = 1.0;
    m->l[SLOPE_AXIS2D_LEFT].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_TOP].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_TOP].line_width = 1.0;
    m->l[SLOPE_AXIS2D_TOP].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_RIGHT].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_RIGHT].line_width = 1.0;
    m->l[SLOPE_AXIS2D_RIGHT].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_X].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_X].line_width = 1.0;
    m->l[SLOPE_AXIS2D_X].options = SLOPE_AXIS2D_DRAW_TICKS;

    m->l[SLOPE_AXIS2D_Y].line_color = SLOPE_BLACK;
    m->l[SLOPE_AXIS2D_Y].line_width = 1.0;
    m->l[SLOPE_AXIS2D_Y].options = SLOPE_AXIS2D_DRAW_TICKS;

    /* Visible frame initialization */
    m->bg_fill_color = SLOPE_WHITE;
    m->bg_stroke_color = SLOPE_MAROON;
    m->options = DrawRect | DrawTitle | RoundedRect;
    m->bg_stroke_width = 2.0;
    m->title = g_strdup("Slope");
    m->title_color = SLOPE_BLACK;
    m->margin = 4;
}


static void
axis2d_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->dispose (object);
}


static void
axis2d_finalize(GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->finalize (object);
}


SlopeItem*
slope_axis2d_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_AXIS2D, NULL));
}


static void
axis2d_draw_rect (SlopeAxis2D *self, cairo_t *cr, SlopeRect *rect)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);

    if (!slope_enabled(m->options, DrawRect) ||
            (!slope_rgba_is_visible(m->bg_fill_color) &&
             !slope_rgba_is_visible(m->bg_stroke_color))) {
        return;
    }

    rect->width -= 2 * m->margin;
    rect->height -= 2 * m->margin;
    cairo_translate (cr, m->margin, m->margin);

    if (slope_enabled(m->options, RoundedRect)) {
        slope_cairo_round_rect (cr, rect, 10);
    } else {
        slope_cairo_rect (cr, rect);
    }

    slope_cairo_draw (
          cr, m->bg_stroke_width,
          m->bg_fill_color, m->bg_stroke_color);
}


static void
axis2d_draw_title (SlopeAxis2D *self,
                   const SlopeItemDC *dc)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    SlopeRect ink, logical;
    cairo_t *cr = dc->cr;

    if (!slope_enabled(m->options, DrawTitle) ||
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
axis2d_draw (SlopeItem *self, const SlopeItemDC *dc)
{
    SlopeRect rec = dc->rect;
    axis2d_draw_rect (SLOPE_AXIS2D (self), dc->cr, &rec);
    axis2d_draw_title (SLOPE_AXIS2D (self), dc);
}

/* slope/axis2d.c */
