/*
 * Copyright (C) 2019  Elvis Teixeira
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
#include "slope/frame.h"
#include "slope/item.h"

typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))

typedef struct _CoordLine CoordLine;
#define COORD_LINE(Addr) ((CoordLine *) (Addr))
#define COORD_LINE_VISIBLE(Addr) (COORD_LINE(Addr)->piece_enable != 0U)

#define COORD_LINE_BOTTOM 0
#define COORD_LINE_LEFT 1
#define COORD_LINE_TOP 2
#define COORD_LINE_RIGHT 3
#define COORD_LINE_X 4
#define COORD_LINE_Y 5

#define COORD_PIECE_NONE    (0U)
#define COORD_PIECE_LINE    (1U)
#define COORD_PIECE_TICKS   (1U << 1U)
#define COORD_PIECE_LABEL   (1U << 2U)
#define COORD_PIECE_ALL     (0xFFFFFFFFU)


struct _CoordLine {
    double line_width;
    SlopeRGBA line_stroke_color;
    guint32 piece_enable;
    SlopePoint p1;
    SlopePoint p2;
    SlopeOrientation orientation;
};


struct _SlopeAxis2DPrivate
{
    double left_margin, right_margin;
    double bottom_margin, top_margin;

    double fig_x_min, fig_x_max;
    double fig_y_min, fig_y_max;
    double fig_width, fig_height;

    double dat_x_min, dat_x_max;
    double dat_y_min, dat_y_max;
    double dat_width, dat_height;

    CoordLine coord_lines[6];
};


#define SLOPE_AXIS2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))


G_DEFINE_TYPE_WITH_PRIVATE (SlopeAxis2D, slope_axis2d, SLOPE_TYPE_FRAME)


/* local decls */
static void axis2d_finalize (GObject *self);
static void axis2d_dispose (GObject *self);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc);
static void axis2d_draw_lines (SlopeAxis2D *self, const SlopeItemDC *dc);
static void axis2d_draw_line (SlopeAxis2D *self, CoordLine *line, const SlopeItemDC *dc);
static void axis2d_draw_piece_line (SlopeAxis2D *self, CoordLine *line, const SlopeItemDC *dc);

static void
slope_axis2d_class_init (SlopeAxis2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = axis2d_dispose;
    gobject_class->finalize = axis2d_finalize;

    item_class->draw_self = axis2d_draw_self;
    item_class->draw_tree = axis2d_draw_tree;

    klass->add_plot = axis2d_add_plot;
}


static void
slope_axis2d_init (SlopeAxis2D *axis)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (axis);
    CoordLine *line;

    slope_axis2d_update_scale (axis);

    m->right_margin = 20.0;
    m->top_margin = 20.0;
    m->bottom_margin = 20.0;
    m->left_margin = 20.0;

    line = &m->coord_lines[COORD_LINE_BOTTOM];
    line->line_stroke_color = SLOPE_BLACK;
    line->piece_enable = COORD_PIECE_ALL;
    line->orientation = SLOPE_BOTTOM;
    line->line_width = 1.0;

    line = &m->coord_lines[COORD_LINE_LEFT];
    line->line_stroke_color = SLOPE_BLACK;
    line->piece_enable = COORD_PIECE_ALL;
    line->orientation = SLOPE_LEFT;
    line->line_width = 1.0;

    line = &m->coord_lines[COORD_LINE_TOP];
    line->line_stroke_color = SLOPE_BLACK;
    line->piece_enable = COORD_PIECE_ALL;
    line->orientation = SLOPE_TOP;
    line->line_width = 1.0;

    line = &m->coord_lines[COORD_LINE_RIGHT];
    line->line_stroke_color = SLOPE_BLACK;
    line->piece_enable = COORD_PIECE_ALL;
    line->orientation = SLOPE_RIGHT;
    line->line_width = 1.0;

    line = &m->coord_lines[COORD_LINE_X];
    line->line_stroke_color = SLOPE_BLACK;
    line->piece_enable = COORD_PIECE_NONE;
    line->orientation = SLOPE_LEFT;
    line->line_width = 1.0;

    line = &m->coord_lines[COORD_LINE_Y];
    line->line_stroke_color = SLOPE_BLACK;
    line->piece_enable = COORD_PIECE_NONE;
    line->orientation = SLOPE_BOTTOM;
    line->line_width = 1.0;
}


static void
axis2d_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->dispose (object);
}


static void
axis2d_finalize (GObject *object)
{
    G_OBJECT_CLASS (slope_axis2d_parent_class)->finalize (object);
}


SlopeItem*
slope_axis2d_new (const char *title)
{
    SlopeItem *self = SLOPE_ITEM (g_object_new (SLOPE_TYPE_AXIS2D, NULL));
    slope_frame_set_title (SLOPE_FRAME (self), title);
    return self;
}


static void
axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    /* draw the frame stuff */
    SLOPE_ITEM_CLASS (slope_axis2d_parent_class)->draw_self (self, dc);
}


static void
axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot)
{
    slope_item_append (SLOPE_ITEM (self), SLOPE_ITEM (plot));
    slope_axis2d_update_scale (self);
}


void
slope_axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot)
{
    g_return_if_fail (SLOPE_IS_AXIS2D (self));
    g_return_if_fail (SLOPE_IS_PLOT2D (plot));
    SLOPE_AXIS2D_GET_CLASS (self)->add_plot (self, plot);
}


void
slope_axis2d_update_scale (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m;
    SlopeTree *iter;
    SlopeItem *item;
    double plt_x_min, plt_x_max;
    double plt_y_min, plt_y_max;

    g_assert (SLOPE_IS_AXIS2D (self));
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    /* init values for the 'no child' case */
    m->dat_x_min = 0.0;
    m->dat_x_max = 1.0;
    m->dat_y_min = 0.0;
    m->dat_y_max = 1.0;
    m->dat_width = 1.0;
    m->dat_height = 1.0;

    if (!(iter = slope_item_get_fisrt_child (SLOPE_ITEM (self)))) {
        /* no subitems */
        return;
    }

    /* init axis extents with the first plot's extents */
    item = slope_item_from_tree_node (iter);
    slope_plot2d_get_data_extents (
                SLOPE_PLOT2D (item),
                &plt_x_min, &plt_x_max,
                &plt_y_min, &plt_y_max);

    m->dat_x_min = plt_x_min;
    m->dat_x_max = plt_x_max;
    m->dat_y_min = plt_y_min;
    m->dat_y_max = plt_y_max;

    iter = iter->next;
    while (iter != NULL) {

        /* update axis extents with the others */
        item = slope_item_from_tree_node (iter);
        slope_plot2d_get_data_extents (
                SLOPE_PLOT2D (item),
                &plt_x_min, &plt_x_max,
                &plt_y_min, &plt_y_max);

        if (plt_x_min < m->dat_x_min) m->dat_x_min = plt_x_min;
        if (plt_x_max > m->dat_x_max) m->dat_x_max = plt_x_max;
        if (plt_y_min < m->dat_y_min) m->dat_y_min = plt_y_min;
        if (plt_y_max > m->dat_y_max) m->dat_y_max = plt_y_max;

        iter = iter->next;
    }

    m->dat_width = m->dat_x_max - m->dat_x_min;
    m->dat_height = m->dat_y_max - m->dat_y_min;
}


void
slope_axis2d_map (SlopeAxis2D *self, SlopePoint *f, const SlopePoint *d)
{
    SlopeAxis2DPrivate *m;

    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (d != NULL && f != NULL);
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    f->x = m->fig_x_min + ((d->x - m->dat_x_min) / m->dat_width) * m->fig_width;
    f->y = m->fig_y_min + ((m->dat_y_max - d->y) / m->dat_height) * m->fig_height;
}


void
slope_axis2d_unmap (SlopeAxis2D *self, SlopePoint *d, const SlopePoint *f)
{
     SlopeAxis2DPrivate *m;

    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (d != NULL && f != NULL);
    m = SLOPE_AXIS2D_GET_PRIVATE (self);

    d->x = m->dat_x_min + ((f->x - m->fig_x_min) / m->fig_width) * m->dat_width;
    d->y = m->dat_y_min + ((m->fig_y_max - d->y) / m->fig_height) * m->dat_height;
}


static void
axis2d_draw_lines (SlopeAxis2D *self, const SlopeItemDC *dc)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    CoordLine *line;
    SlopePoint dat_pt = {0.0, 0.0}, fig_pt = {0.0, 0.0};

    slope_axis2d_map (self, &fig_pt, &dat_pt);

    /* The 0.5 added to the lin lengths make sure the
     * corner pixel on the frame will not be left blank */

    line = &m->coord_lines[COORD_LINE_BOTTOM];
    line->p1.x = m->fig_x_min;
    line->p1.y = m->fig_y_max;
    line->p2.x = m->fig_x_max + 0.5;
    line->p2.y = m->fig_y_max;
    axis2d_draw_line (self, line, dc);

    line = &m->coord_lines[COORD_LINE_LEFT];
    line->p1.x = m->fig_x_min;
    line->p1.y = m->fig_y_max;
    line->p2.x = m->fig_x_min;
    line->p2.y = m->fig_y_min - 0.5;
    axis2d_draw_line (self, line, dc);

    line = &m->coord_lines[COORD_LINE_TOP];
    line->p1.x = m->fig_x_min;
    line->p1.y = m->fig_y_min;
    line->p2.x = m->fig_x_max + 0.5;
    line->p2.y = m->fig_y_min;
    axis2d_draw_line (self, line, dc);

    line = &m->coord_lines[COORD_LINE_RIGHT];
    line->p1.x = m->fig_x_max;
    line->p1.y = m->fig_y_max;
    line->p2.x = m->fig_x_max;
    line->p2.y = m->fig_y_min - 0.5;
    axis2d_draw_line (self, line, dc);

    line = &m->coord_lines[COORD_LINE_X];
    line->p1.x = m->fig_x_min;
    line->p1.y = fig_pt.y;
    line->p2.x = m->fig_x_max + 0.5;
    line->p2.y = fig_pt.y;
    axis2d_draw_line (self, line, dc);

    line = &m->coord_lines[COORD_LINE_Y];
    line->p1.x = fig_pt.x;
    line->p1.y = m->fig_y_max;
    line->p2.x = fig_pt.x;
    line->p2.y = m->fig_y_min - 0.5;
    axis2d_draw_line (self, line, dc);
}


static void
axis2d_draw_line (SlopeAxis2D *self, CoordLine *line, const SlopeItemDC *dc)
{
    SlopeRGBA bg_fill = slope_frame_get_background_fill_color (SLOPE_FRAME (self));

    if (slope_enabled(line->piece_enable, COORD_PIECE_LINE)
            && slope_rgba_is_visible(line->line_stroke_color)
            && line->line_stroke_color != bg_fill) {
        axis2d_draw_piece_line(self, line, dc);
    }
}


static void
axis2d_draw_piece_line (SlopeAxis2D *self, CoordLine *line, const SlopeItemDC *dc)
{
    cairo_antialias_t antialias = cairo_get_antialias(dc->cr);
    SLOPE_UNUSED(self);
    cairo_set_antialias(dc->cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width(dc->cr, line->line_width);
    slope_cairo_set_rgba(dc->cr, line->line_stroke_color);
    cairo_move_to(dc->cr, line->p1.x, line->p1.y);
    cairo_line_to(dc->cr, line->p2.x, line->p2.y);
    cairo_stroke(dc->cr);
    cairo_set_antialias(dc->cr, antialias);
}


static void
axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    int margin = slope_frame_get_margin (SLOPE_FRAME (self));
    SlopeRect orig_rect = dc->rect;

    cairo_save (dc->cr);

    /* If stuff like margins are to be applyed, we need to
     * adjust the rectangle to which subitems will have access to */
    if (margin > 0) {
        dc->rect.x += margin;
        dc->rect.y += margin;
        dc->rect.width -= 2 * margin;
        dc->rect.height -= 2 * margin;
    }

    slope_cairo_rect (dc->cr, &dc->rect);
    cairo_clip (dc->cr);

    m->fig_x_min = dc->rect.x + m->left_margin;
    m->fig_x_max = dc->rect.x + dc->rect.width - m->right_margin;
    m->fig_width = m->fig_x_max - m->fig_x_min;

    m->fig_y_min = dc->rect.y + m->top_margin;
    m->fig_y_max = dc->rect.y + dc->rect.height - m->bottom_margin;
    m->fig_height = m->fig_y_max - m->fig_y_min;

    slope_frame_draw_rect (SLOPE_FRAME (self), dc);
    SLOPE_ITEM_GET_CLASS (self)->draw_children (self, dc);
    axis2d_draw_lines (SLOPE_AXIS2D (self), dc);
    slope_frame_draw_title (SLOPE_FRAME (self), dc);

    cairo_restore (dc->cr);
    dc->rect = orig_rect;
}

/* slope/axis2d.c */
