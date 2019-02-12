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
#include "slope/sampler.h"
#include <stdio.h>

#define MIN_PIXLEN 10.0


typedef struct _SlopeAxis2DPrivate SlopeAxis2DPrivate;
#define SLOPE_AXIS2D_PRIVATE(Addr) ((SlopeAxis2DPrivate*) (Addr))

typedef enum {
    SCALE_BOTTOM,
    SCALE_LEFT,
    SCALE_TOP,
    SCALE_RIGHT,
    SCALE_X,
    SCALE_Y,
    SCALE_COUNT
} ScaleIndex;


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

    SlopeItem *scales[SCALE_COUNT];
};


#define SLOPE_AXIS2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_AXIS2D, SlopeAxis2DPrivate))


G_DEFINE_TYPE_WITH_PRIVATE (SlopeAxis2D, slope_axis2d, SLOPE_TYPE_FRAME)


/* local decls */
static void axis2d_finalize (GObject *self);
static void axis2d_dispose (GObject *self);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc);
static void axis2d_set_scales_position (SlopeAxis2D *self);
static void axis2d_add_bottom (SlopeItem *parent, SlopeItem *child);
static void axis2d_add_top (SlopeItem *parent, SlopeItem *child);
static void base_add_top (SlopeItem *self, SlopeItem *child);
static void base_add_bottom (SlopeItem *self, SlopeItem *child);
static void axis2d_set_x_scale_samples (SlopeAxis2D *self, SlopeScale *scale);
static void axis2d_set_y_scale_samples (SlopeAxis2D *self, SlopeScale *scale);


static void
slope_axis2d_class_init (SlopeAxis2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = axis2d_dispose;
    gobject_class->finalize = axis2d_finalize;

    item_class->draw_self = axis2d_draw_self;
    item_class->draw_tree = axis2d_draw_tree;
    item_class->add_top = axis2d_add_top;
    item_class->add_bottom = axis2d_add_bottom;
}


static void
slope_axis2d_init (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    guint k;

    slope_axis2d_update_scale (self);

    m->right_margin = 20.0;
    m->top_margin = 20.0;
    m->bottom_margin = 20.0;
    m->left_margin = 20.0;

    for (k = 0; k < SCALE_COUNT; ++k) {
        m->scales[k] = slope_scale_new(SLOPE_SCALE_ANTIALIAS);
        base_add_top (SLOPE_ITEM (self), m->scales[k]);
    }

    slope_scale_set_trait (SLOPE_SCALE (m->scales[SCALE_LEFT]), SLOPE_SCALE_REVERSE, TRUE);
    slope_scale_set_trait (SLOPE_SCALE (m->scales[SCALE_Y]), SLOPE_SCALE_REVERSE, TRUE);
    slope_scale_set_trait (SLOPE_SCALE (m->scales[SCALE_TOP]), SLOPE_SCALE_REVERSE, TRUE);
    slope_axis2d_set_scales (SLOPE_AXIS2D (self), SLOPE_AXIS2D_SCALE_ALL);
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


static void
put_scales_on_top (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    SlopeTree *node, *tree;
    guint k;

    tree = slope_item_get_tree_node (SLOPE_ITEM (self));

    for (k = 0; k < SCALE_COUNT; ++k) {
        node = slope_item_get_tree_node (m->scales[k]);
        slope_tree_append (tree, slope_tree_detach (tree, node));
    }
}


static void
base_add_top (SlopeItem *self, SlopeItem *child)
{
    SLOPE_ITEM_CLASS (slope_axis2d_parent_class)->add_top (self, child);
}


static void
base_add_bottom (SlopeItem *self, SlopeItem *child)
{
    SLOPE_ITEM_CLASS (slope_axis2d_parent_class)->add_bottom (self, child);
}


static void
axis2d_add_top (SlopeItem *self, SlopeItem *child)
{
    base_add_top (self, child);
    put_scales_on_top (SLOPE_AXIS2D (self));
    slope_axis2d_update_scale (SLOPE_AXIS2D (self));
}


static void
axis2d_add_bottom (SlopeItem *self, SlopeItem *child)
{
    base_add_bottom (self, child);
    put_scales_on_top (SLOPE_AXIS2D (self));
    slope_axis2d_update_scale (SLOPE_AXIS2D (self));
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


SlopeScale* slope_axis2d_get_scale (SlopeAxis2D *self, SlopeAxisScale scale)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    g_assert (SLOPE_IS_AXIS2D (self));

    switch (scale) {
        case SLOPE_AXIS2D_SCALE_BOTTOM: return SLOPE_SCALE(m->scales[SCALE_BOTTOM]);
        case SLOPE_AXIS2D_SCALE_LEFT: return SLOPE_SCALE(m->scales[SCALE_LEFT]);
        case SLOPE_AXIS2D_SCALE_TOP: return SLOPE_SCALE(m->scales[SCALE_TOP]);
        case SLOPE_AXIS2D_SCALE_RIGHT: return SLOPE_SCALE(m->scales[SCALE_RIGHT]);
        case SLOPE_AXIS2D_SCALE_X: return SLOPE_SCALE(m->scales[SCALE_X]);
        case SLOPE_AXIS2D_SCALE_Y: return SLOPE_SCALE(m->scales[SCALE_Y]);
    }

    g_assert (FALSE);
    return NULL;
}


void slope_axis2d_set_scales (SlopeAxis2D *self, SlopeAxisScale scales)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);

    g_assert (SLOPE_IS_AXIS2D (self));
    g_assert (scales >= 0
              && scales <= (SLOPE_AXIS2D_SCALE_BOTTOM
                            |SLOPE_AXIS2D_SCALE_LEFT
                            |SLOPE_AXIS2D_SCALE_TOP
                            |SLOPE_AXIS2D_SCALE_RIGHT
                            |SLOPE_AXIS2D_SCALE_X
                            |SLOPE_AXIS2D_SCALE_Y));

    slope_item_set_visible (m->scales[SCALE_BOTTOM], slope_enabled(scales, SLOPE_AXIS2D_SCALE_BOTTOM));
    slope_item_set_visible (m->scales[SCALE_LEFT], slope_enabled(scales, SLOPE_AXIS2D_SCALE_LEFT));
    slope_item_set_visible (m->scales[SCALE_TOP], slope_enabled(scales, SLOPE_AXIS2D_SCALE_TOP));
    slope_item_set_visible (m->scales[SCALE_RIGHT], slope_enabled(scales, SLOPE_AXIS2D_SCALE_RIGHT));
    slope_item_set_visible (m->scales[SCALE_X], slope_enabled(scales, SLOPE_AXIS2D_SCALE_X));
    slope_item_set_visible (m->scales[SCALE_Y], slope_enabled(scales, SLOPE_AXIS2D_SCALE_Y));
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

    if (NULL == (iter = slope_item_get_fisrt_child (SLOPE_ITEM (self)))) {
        /* no subitems */
        return;
    }

    /* Init axis extents with the first plot's extents */
    for (; iter != NULL; iter = iter->next) {
        item = slope_item_from_tree_node (iter);
        if (SLOPE_IS_PLOT2D(item)) {

            slope_plot2d_get_data_extents (
                    SLOPE_PLOT2D (item),
                    &plt_x_min, &plt_x_max,
                    &plt_y_min, &plt_y_max);

            m->dat_x_min = plt_x_min;
            m->dat_x_max = plt_x_max;
            m->dat_y_min = plt_y_min;
            m->dat_y_max = plt_y_max;
            iter = iter->next;
            break;
        }
    }

    /* Update axis extents with the others */
    for (; iter != NULL; iter = iter->next) {
        item = slope_item_from_tree_node (iter);
        if (SLOPE_IS_PLOT2D(item)) {

            slope_plot2d_get_data_extents (
                    SLOPE_PLOT2D (item),
                    &plt_x_min, &plt_x_max,
                    &plt_y_min, &plt_y_max);

            if (plt_x_min < m->dat_x_min) m->dat_x_min = plt_x_min;
            if (plt_x_max > m->dat_x_max) m->dat_x_max = plt_x_max;
            if (plt_y_min < m->dat_y_min) m->dat_y_min = plt_y_min;
            if (plt_y_max > m->dat_y_max) m->dat_y_max = plt_y_max;
        }
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
axis2d_set_scales_position (SlopeAxis2D *self)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    SlopePoint fig_p1, fig_p2, zero;
    SlopeScale *scale;

    fig_p2.x = 0;
    fig_p2.y = 0;
    slope_axis2d_map (self, &zero, &fig_p2);

#define SETUP_SCALE_DRAW(LABEL,ORIENT,X1,Y1,X2,Y2) \
    G_STMT_START { \
        fig_p1.x = X1; \
        fig_p1.y = Y1; \
        fig_p2.x = X2; \
        fig_p2.y = Y2; \
        scale = SLOPE_SCALE (m->scales[SCALE_##LABEL]); \
        slope_scale_set_figure_position (scale, &fig_p1, &fig_p2); \
        axis2d_set_##ORIENT##_scale_samples (self, scale); \
    } G_STMT_END

    SETUP_SCALE_DRAW (
        BOTTOM, x,                   /* Location and orientation */
        m->fig_x_min, m->fig_y_max,  /* Initial point (x1,y1) */
        m->fig_x_max, m->fig_y_max); /* Terminal point (x2,y2) */
    SETUP_SCALE_DRAW (
        LEFT, y,                     /* Location and orientation */
        m->fig_x_min, m->fig_y_max,  /* Initial point (x1,y1) */
        m->fig_x_min, m->fig_y_min); /* Terminal point (x2,y2) */
    SETUP_SCALE_DRAW (
        TOP, x,                      /* Location and orientation */
        m->fig_x_min, m->fig_y_min,  /* Initial point (x1,y1) */
        m->fig_x_max, m->fig_y_min); /* Terminal point (x2,y2) */
    SETUP_SCALE_DRAW (
        RIGHT, y,                    /* Location and orientation */
        m->fig_x_max, m->fig_y_max,  /* Initial point (x1,y1) */
        m->fig_x_max, m->fig_y_min); /* Terminal point (x2,y2) */
    SETUP_SCALE_DRAW (
        X, x,                        /* Location and orientation */
        m->fig_x_min, zero.y,        /* Initial point (x1,y1) */
        m->fig_x_max, zero.y);       /* Terminal point (x2,y2) */
    SETUP_SCALE_DRAW (
        Y, y,                        /* Location and orientation */
        zero.x, m->fig_y_max,        /* Initial point (x1,y1) */
        zero.x, m->fig_y_min);       /* Terminal point (x2,y2) */
#undef SETUP_SCALE_DRAW
}


static void
axis2d_draw_tree (SlopeItem *self, SlopeItemDC *dc)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    double frame_margin = slope_frame_get_margin (SLOPE_FRAME (self));
    SlopeRect orig_rect = dc->rect;

    cairo_save (dc->cr);

    /* If stuff like margins are to be applyed, we need to
     * adjust the rectangle to which subitems will have access to */
    if (frame_margin > 0) {
        dc->rect.x += frame_margin;
        dc->rect.y += frame_margin;
        dc->rect.width -= 2 * frame_margin;
        dc->rect.height -= 2 * frame_margin;
    }

    slope_cairo_rect (dc->cr, &dc->rect);
    cairo_clip (dc->cr);

    m->fig_x_min = dc->rect.x + m->left_margin;
    m->fig_x_max = dc->rect.x + dc->rect.width - m->right_margin;
    m->fig_width = m->fig_x_max - m->fig_x_min;

    if (MIN_PIXLEN > m->fig_width) {
        g_warning("Slope", G_LOG_LEVEL_ERROR, "Axis2D has improper range");
        goto cleanup;
    }

    m->fig_y_min = dc->rect.y + m->top_margin;
    m->fig_y_max = dc->rect.y + dc->rect.height - m->bottom_margin;
    m->fig_height = m->fig_y_max - m->fig_y_min;

    if (MIN_PIXLEN > m->fig_height) {
        g_warning("Slope", G_LOG_LEVEL_ERROR, "Axis2D has improper range");
        goto cleanup;
    }

    slope_frame_draw_rect (SLOPE_FRAME (self), dc);
    axis2d_set_scales_position (SLOPE_AXIS2D (self));
    SLOPE_ITEM_GET_CLASS (self)->draw_children (self, dc);
    slope_frame_draw_title (SLOPE_FRAME (self), dc);

cleanup:
    cairo_restore (dc->cr);
    dc->rect = orig_rect;
}


static const char *const TICK_FORMAT[] = {"%2.1f", "%2.1e"};


static void
axis2d_set_x_scale_samples (SlopeAxis2D *self, SlopeScale *scale)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    SlopeSampler *sampler;
    double min = m->dat_x_min;
    double max = m->dat_x_max;
    double divs = m->fig_width / 80.0;
    double fig_step, dat_step;
    guint k = 0;
    SlopePoint f, d;
    SlopeSample sample;

    slope_sample_guess_decimal_spacing (&min, &max, &divs);
    sampler = slope_scale_get_sampler (scale);
    slope_sampler_clear (sampler);

    d.x = min;
    d.y = 0.0;
    slope_axis2d_map (self, &f, &d);
    fig_step = f.x;

    d.x = max;
    d.y = 0.0;
    slope_axis2d_map (self, &f, &d);

    fig_step = f.x - fig_step;
    dat_step = (max - min) / divs;
    d.x = min;

    for (k = 0; k < divs; ++k) {
        sample.value = f.x / m->fig_width;
        sprintf (sample.label, TICK_FORMAT[0], d.x);
        slope_sampler_add_sample (sampler, &sample);
        f.x += fig_step;
        d.x += dat_step;
    }
}


static void
axis2d_set_y_scale_samples (SlopeAxis2D *self, SlopeScale *scale)
{
    SlopeAxis2DPrivate *m = SLOPE_AXIS2D_GET_PRIVATE (self);
    SlopeSampler *sampler;
    double min = m->dat_y_min;
    double max = m->dat_y_max;
    double divs = m->fig_height / 80.0;
    double fig_step, dat_step;
    guint k = 0;
    SlopePoint f, d;
    SlopeSample sample;

    slope_sample_guess_decimal_spacing (&min, &max, &divs);
    sampler = slope_scale_get_sampler (scale);
    slope_sampler_clear (sampler);

    d.x = 0.0;
    d.y = min;
    slope_axis2d_map (self, &f, &d);
    fig_step = f.y;

    d.x = 0.0;
    d.y = max;
    slope_axis2d_map (self, &f, &d);

    fig_step = f.y - fig_step;
    dat_step = (max - min) / divs;
    d.y = min;

    for (k = 0; k < divs; ++k) {
        sample.value = f.y / m->fig_width;
        sprintf (sample.label, TICK_FORMAT[0], d.y);
        slope_sampler_add_sample (sampler, &sample);
        f.x += fig_step;
        d.y += dat_step;
    }
}

/* slope/axis2d.c */
