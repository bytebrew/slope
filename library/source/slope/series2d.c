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

#include "slope/series2d.h"
#include "slope/axis2d.h"
#include "slope/item.h"


typedef struct _SlopeSeries2DPrivate SlopeSeries2DPrivate;
#define SLOPE_SERIES2D_PRIVATE(Addr) ((SlopeSeries2DPrivate*) (Addr))


struct _SlopeSeries2DPrivate
{
    double x_min, x_max;
    double y_min, y_max;

    SlopeArray2D *array;

    guint32 options;
    guint32 marker;
    double marker_radius;

    SlopeRGBA line_color;
    SlopeRGBA marker_color;
    double line_width;
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeSeries2D, slope_series2d, SLOPE_TYPE_PLOT2D)

#define SLOPE_SERIES2D_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
    SLOPE_TYPE_SERIES2D, SlopeSeries2DPrivate))


/* local decls */
static void series2d_finalize (GObject *self);
static void series2d_dispose (GObject *self);
static void series2d_draw_self (SlopeItem *self, const SlopeItemDC *dc);
static void series2d_get_data_extents (SlopePlot2D *self,
                                       double *x_min, double *x_max,
                                       double *y_min, double *y_max);


static void
slope_series2d_class_init (SlopeSeries2DClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    SlopeItemClass *item_class = SLOPE_ITEM_CLASS (klass);
    SlopePlot2DClass *plt2d_class = SLOPE_PLOT2D_CLASS (klass);

    gobject_class->dispose = series2d_dispose;
    gobject_class->finalize = series2d_finalize;

    item_class->draw_self = series2d_draw_self;

    plt2d_class->get_data_extents = series2d_get_data_extents;
}


static void
slope_series2d_init (SlopeSeries2D *axis)
{
    SlopeSeries2DPrivate *m = SLOPE_SERIES2D_GET_PRIVATE (axis);

    m->array = NULL;

    m->options =
            SLOPE_SERIES2D_OWN_ARRAY |
            SLOPE_SERIES2D_DRAW_LINE |
            SLOPE_SERIES2D_DRAW_MARKER |
            SLOPE_SERIES2D_LINE_ANTIALIAS |
            SLOPE_SERIES2D_MARKER_ANTIALIAS;

    m->x_min = 0.0;
    m->x_max = 1.0;
    m->y_min = 0.0;
    m->y_max = 1.0;

    m->line_color = SLOPE_BLACK;
    m->line_width = 1.0;

    m->marker = SLOPE_SERIES2D_CIRCLE;
    m->marker_radius = 3.0;
}


static void
series2d_dispose (GObject *object)
{
    slope_series2d_set_data (SLOPE_SERIES2D (object), NULL, TRUE);
    G_OBJECT_CLASS (slope_series2d_parent_class)->dispose (object);
}


static void
series2d_finalize (GObject *object)
{
    G_OBJECT_CLASS (slope_series2d_parent_class)->finalize (object);
}


SlopeItem*
slope_series2d_new (void)
{
    return SLOPE_ITEM (g_object_new (SLOPE_TYPE_SERIES2D, NULL));
}


void
slope_series2d_format (SlopeSeries2D *self, const char *format)
{
    const char *c = format;
    SlopeSeries2DPrivate *m;
    SlopeRGBA color = SLOPE_BLACK;

    g_return_if_fail (SLOPE_IS_SERIES2D (self));
    m = SLOPE_SERIES2D_GET_PRIVATE (self);

    slope_disable(m->options, SLOPE_SERIES2D_DRAW_LINE);
    slope_disable(m->options, SLOPE_SERIES2D_DRAW_MARKER);

    for (; *c; ++c) {

        /* get a color */
        switch (*c) {
            case 'w': color = SLOPE_WHITE; break;
            case 'k': color = SLOPE_BLACK; break;
            case 'r': color = SLOPE_RED; break;
            case 'g': color = SLOPE_GREEN; break;
            case 'b': color = SLOPE_BLUE; break;

            case '-': {
                slope_enable(m->options, SLOPE_SERIES2D_DRAW_LINE);
                m->line_color = color;
                break;
            }
            case 'o': {
                slope_enable(m->options, SLOPE_SERIES2D_DRAW_MARKER);
                m->marker = SLOPE_SERIES2D_CIRCLE;
                m->marker_color = color;
                break;
            }
        }
    }
}


SlopeItem*
slope_series2d_new_formatted (const char *format)
{
    SlopeSeries2D *self = g_object_new (SLOPE_TYPE_SERIES2D, NULL);

    slope_series2d_format (self, format);
    return SLOPE_ITEM (self);
}


static void
series2d_draw_self (SlopeItem *self, const SlopeItemDC *dc)
{
    gulong k, npts;
    SlopePoint *pts, f, d;
    SlopeSeries2DPrivate *m;
    SlopeAxis2D *axis;

    g_return_if_fail (SLOPE_IS_SERIES2D (self));

    m = SLOPE_SERIES2D_GET_PRIVATE (self);
    axis = SLOPE_AXIS2D (slope_item_get_parent (self));

    slope_array2d_get_points (m->array, &npts, &pts);
    if (npts == 0L) return;

    d.x = pts[0].x;
    d.y = pts[0].y;
    slope_axis2d_map (SLOPE_AXIS2D (axis), &f, &d);
    cairo_move_to (dc->cr, f.x, f.y);

    for (k = 1L; k < npts; ++k) {

        d.x = pts[k].x;
        d.y = pts[k].y;
        slope_axis2d_map (SLOPE_AXIS2D (axis), &f, &d);
        cairo_line_to (dc->cr, f.x, f.y);
    }

    if (slope_enabled(m->options, SLOPE_SERIES2D_LINE_ANTIALIAS)) {
        cairo_set_antialias(dc->cr, CAIRO_ANTIALIAS_GOOD);
    } else {
        cairo_set_antialias(dc->cr, CAIRO_ANTIALIAS_NONE);
    }

    cairo_set_line_width (dc->cr, m->line_width);
    slope_cairo_set_rgba (dc->cr, m->line_color);
    cairo_stroke (dc->cr);
}


static void
series2d_get_data_extents (SlopePlot2D *self,
                           double *x_min, double *x_max,
                           double *y_min, double *y_max)
{
    SlopeSeries2DPrivate *m;
    g_return_if_fail (SLOPE_IS_SERIES2D (self));

    m = SLOPE_SERIES2D_GET_PRIVATE (self);
    *x_min = m->x_min;
    *x_max = m->x_max;
    *y_min = m->y_min;
    *y_max = m->y_max;
}


void slope_series2d_set_data (SlopeSeries2D *self,
                              SlopeArray2D *array, gboolean own)
{
    SlopeSeries2DPrivate *m;
    SlopePoint *pts;
    gulong k, npts;

    g_return_if_fail (SLOPE_IS_SERIES2D (self));
    m = SLOPE_SERIES2D_GET_PRIVATE (self);

    /* Default values for no data */
    m->x_min = 0.0;
    m->x_max = 1.0;
    m->y_min = 0.0;
    m->y_max = 1.0;

    if (m->array != NULL && slope_enabled(m->options, SLOPE_SERIES2D_OWN_ARRAY)) {
        slope_array2d_delete (m->array);
    }

    m->array = array;
    slope_enable_if (m->options, own, SLOPE_SERIES2D_OWN_ARRAY);

    if (m->array != NULL) {
        slope_array2d_get_points (m->array, &npts, &pts);

        if (npts > 0) {
            m->x_min = m->x_max = pts[0].x;
            m->y_min = m->y_max = pts[0].y;

            for (k = 0; k < npts; ++k) {
                if (pts[k].x < m->x_min) m->x_min = pts[k].x;
                if (pts[k].x > m->x_max) m->x_max = pts[k].x;
                if (pts[k].y < m->y_min) m->y_min = pts[k].y;
                if (pts[k].y > m->y_max) m->y_max = pts[k].y;
            }
        }
    }
}

/* slope/series2d.c */
