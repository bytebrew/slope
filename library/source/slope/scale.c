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

#include "slope/scale.h"
#include "slope/color.h"
#include "slope/sampler.h"

typedef struct _SlopeScalePrivate SlopeScalePrivate;
#define SLOPE_SCALE_PRIVATE(Addr) ((SlopeScalePrivate*) (Addr))


struct _SlopeScalePrivate
{
    SlopePoint fig_p1;
    SlopePoint fig_p2;
    double data_min;
    double data_max;

    SlopeRGBA line_color;
    SlopeRGBA line_color_selected;
    double line_width;
    double line_width_selected;

    SlopeSampler sampler;

    gulong show_ticks : 1;
    gulong reverse_ticks : 1;
    gulong antialias : 1;
};


#define SLOPE_SCALE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_SCALE, SlopeScalePrivate))


G_DEFINE_TYPE_WITH_PRIVATE (SlopeScale, slope_scale, SLOPE_TYPE_ITEM)


/* local decls */
static void scale_finalize (GObject *self);
static void scale_dispose (GObject *self);
static void scale_draw_self (SlopeItem *self, const SlopeItemDC *dc);


static void
slope_scale_class_init (SlopeScaleClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = scale_dispose;
    gobject_class->finalize = scale_finalize;

    item_class->draw_self = scale_draw_self;
}


static void
slope_scale_init (SlopeScale *scale)
{
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (scale);

    m->line_color = slope_gray(200);
    m->line_width = 1.0;
    m->line_color_selected = SLOPE_BLUE;
    m->line_width_selected = 2.0;
    m->show_ticks = TRUE;
    m->reverse_ticks = FALSE;
    m->antialias = TRUE;
    m->data_min = 0.0;
    m->data_max = 1.0;

    slope_sampler_init(&m->sampler);
}


static void
scale_dispose (GObject *object)
{
    G_OBJECT_CLASS (slope_scale_parent_class)->dispose (object);
}


static void
scale_finalize (GObject *object)
{
    G_OBJECT_CLASS (slope_scale_parent_class)->finalize (object);
}


SlopeItem*
slope_scale_new ()
{
    SlopeItem *self = SLOPE_ITEM (g_object_new (SLOPE_TYPE_SCALE, NULL));
    return self;
}


void
slope_scale_set_figure_position (SlopeScale *self, const SlopePoint *p1, const SlopePoint *p2)
{
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (self);

    g_assert (SLOPE_IS_SCALE (self));
    g_assert (p1 && p2);

    m->fig_p1 = *p1;
    m->fig_p2 = *p2;
}


void
slope_scale_set_data_extents (SlopeScale *self, double min, double max)
{
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (self);

    g_assert (SLOPE_IS_SCALE (self));
    g_assert (min < max);

    m->data_min = min;
    m->data_max = max;
}


static void
scale_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (self);
    cairo_antialias_t antialias = CAIRO_ANTIALIAS_GOOD;
    double fig_dy = m->fig_p2.y - m->fig_p1.y;
    double fig_dx = m->fig_p2.x - m->fig_p1.x;

    if (!m->antialias
            || slope_float_similar_zero(fig_dx)
            || slope_float_similar_zero(fig_dy)) {
        antialias = CAIRO_ANTIALIAS_NONE;
    }

    if (slope_color_is_visible(m->line_color)) {
        slope_cairo_set_rgba (dc->cr, m->line_color);
        cairo_set_antialias (dc->cr, antialias);
        cairo_set_line_width (dc->cr, m->line_width);
        cairo_move_to (dc->cr, m->fig_p1.x, m->fig_p1.y);
        cairo_line_to (dc->cr, m->fig_p2.x, m->fig_p2.y);
        cairo_stroke (dc->cr);
    }
}

/* slope/scale.c */
