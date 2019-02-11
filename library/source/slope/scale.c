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
#include "math.h"

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
    SlopeRGBA label_color;
    SlopeRGBA label_color_selected;
    double line_width;
    double line_width_selected;
    double tick_length;

    SlopeSampler sampler;

    gulong traits;
};


#define SLOPE_SCALE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_SCALE, SlopeScalePrivate))


G_DEFINE_TYPE_WITH_PRIVATE (SlopeScale, slope_scale, SLOPE_TYPE_ITEM)


/* local decls */
static void scale_finalize (GObject *self);
static void scale_dispose (GObject *self);
static void scale_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void axis2d_toggle_highlight (SlopeItem *self);


static void
slope_scale_class_init (SlopeScaleClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);

    gobject_class->dispose = scale_dispose;
    gobject_class->finalize = scale_finalize;

    item_class->draw_self = scale_draw_self;
    item_class->toggle_highlight = axis2d_toggle_highlight;
}


static void
slope_scale_init (SlopeScale *scale)
{
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (scale);

    m->line_color = slope_gray(210);
    m->line_width = 2.0;
    m->line_color_selected = SLOPE_CADETBLUE;
    m->line_width_selected = 4.0;
    m->label_color = slope_gray(100);
    m->label_color_selected = slope_gray(100);
    m->traits = SLOPE_SCALE_ANTIALIAS;
    m->data_min = 0.0;
    m->data_max = 1.0;
    m->tick_length = 7.0;

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
slope_scale_new (SlopeScaleTrait traits)
{
    SlopeItem *self = SLOPE_ITEM (g_object_new (SLOPE_TYPE_SCALE, NULL));
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (self);
    m->traits = traits;
    return self;
}


void
slope_scale_set_trait (SlopeScale *self, SlopeScaleTrait trait, gboolean toggle)
{
    SlopeScalePrivate *m;

    g_assert (SLOPE_IS_SCALE (self));
    m = SLOPE_SCALE_GET_PRIVATE (self);

    slope_enable_if(m->traits, toggle, trait);
}


SlopeScaleTrait
slope_scale_get_traits (SlopeScale *self)
{
    SlopeScalePrivate *m;

    g_assert (SLOPE_IS_SCALE (self));
    m = SLOPE_SCALE_GET_PRIVATE (self);

    return (SlopeScaleTrait) m->traits;
}


static void
axis2d_toggle_highlight (SlopeItem *self)
{
    SlopeScalePrivate *m;
    SlopeRGBA tmpcolor;
    double tmpfloat;

    g_assert (SLOPE_IS_SCALE (self));
    m = SLOPE_SCALE_GET_PRIVATE (self);

    tmpcolor = m->line_color;
    m->line_color = m->line_color_selected;
    m->line_color_selected = tmpcolor;

    tmpcolor = m->label_color;
    m->label_color = m->label_color_selected;
    m->label_color_selected = tmpcolor;

    tmpfloat = m->line_width;
    m->line_width = m->line_width_selected;
    m->line_width_selected = tmpfloat;
}


void
slope_scale_set_figure_position (SlopeScale *self,
                                 const SlopePoint *p1,
                                 const SlopePoint *p2)
{
    SlopeScalePrivate *m;

    g_assert (SLOPE_IS_SCALE (self));
    g_assert (p1 && p2);

    m = SLOPE_SCALE_GET_PRIVATE (self);
    m->fig_p1 = *p1;
    m->fig_p2 = *p2;
}


void
slope_scale_set_data_extents (SlopeScale *self, double min, double max)
{
    SlopeScalePrivate *m;

    g_assert (SLOPE_IS_SCALE (self));
    g_assert (min < max);

    m = SLOPE_SCALE_GET_PRIVATE (self);
    m->data_min = min;
    m->data_max = max;
}


static void
scale_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    SlopeScalePrivate *m = SLOPE_SCALE_GET_PRIVATE (self);
    cairo_antialias_t antialias = CAIRO_ANTIALIAS_GOOD;
    SlopePoint line, direc, ortog;
    double length, step;

    slope_point_get_diff(line, m->fig_p2, m->fig_p1);
    slope_point_get_normalized(direc, line);
    length = slope_point_length(line);

    if (slope_enabled(m->traits, SLOPE_SCALE_REVERSE_TICKS)) {
        slope_point_get_clock_ortogonal(ortog, direc);
    } else {
        slope_point_get_anticlock_ortogonal(ortog, direc);
    }

    if (FALSE == slope_enabled(m->traits, SLOPE_SCALE_ANTIALIAS)
            || slope_float_similar_zero(direc.x)
            || slope_float_similar_zero(direc.y)) {
        antialias = CAIRO_ANTIALIAS_NONE;
    }

    /* main line */
    if (slope_color_is_visible(m->line_color)) {
        slope_cairo_set_rgba (dc->cr, m->line_color);
        cairo_set_antialias (dc->cr, antialias);
        cairo_set_line_width (dc->cr, m->line_width);
        cairo_move_to (dc->cr, m->fig_p1.x, m->fig_p1.y);
        cairo_line_to (dc->cr, m->fig_p2.x, m->fig_p2.y);
        cairo_stroke (dc->cr);
    }

    /* sample ticks */
    if (0 < m->sampler.size) {
        SlopePoint p1 = m->fig_p1;
        SlopePoint p2 = p1;
        gulong k, n = m->sampler.size;
        SlopeSample *samples = m->sampler.samples;
        step = length / n;

        slope_point_move(p2, m->tick_length, ortog);

        for (k = 0; k < n; ++k) {
            SlopeRect ink, logical;

            slope_cairo_set_rgba (dc->cr, m->line_color);
            cairo_move_to(dc->cr, p1.x, p1.y);
            cairo_line_to(dc->cr, p2.x, p2.y);
            cairo_stroke(dc->cr);

            slope_cairo_set_rgba (dc->cr, m->label_color);
            slope_text_set (dc->text, samples[k].label);
            slope_text_get_extents (dc->text, &ink, &logical);
            cairo_move_to (dc->cr, p1.x - ink.width/2.0, p1.y + 4.0);
            slope_text_show (dc->text);

            slope_point_move(p1, step, direc);
            slope_point_move(p2, step, direc);
        }
    }
}

/* slope/scale.c */
