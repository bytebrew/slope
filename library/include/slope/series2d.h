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

#ifndef _slope_series2d_h_
#define _slope_series2d_h_

#include "slope/plot2d.h"
#include "slope/arrays.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_SERIES2D                 (slope_series2d_get_type ())
#define SLOPE_SERIES2D(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_SERIES2D, SlopeSeries2D))
#define SLOPE_SERIES2D_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_SERIES2D, SlopeSeries2DClass))
#define SLOPE_IS_SERIES2D(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_SERIES2D))
#define SLOPE_IS_SERIES2D_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_SERIES2D))
#define SLOPE_SERIES2D_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_SERIES2D, SlopeSeries2DClass))

typedef struct _SlopeSeries2D             SlopeSeries2D;
typedef struct _SlopeSeries2DClass        SlopeSeries2DClass;


typedef enum {
    SLOPE_SERIES2D_CIRCLE       = 0,
    SLOPE_SERIES2D_SQUARE       = 1,
    SLOPE_SERIES2D_STAR         = 2,
    SLOPE_SERIES2D_TRIANGLE     = 3,
    SLOPE_SERIES2D_DOT          = 4,
} SlopeSeries2DMarker;


typedef enum {
    SLOPE_SERIES2D_NONE               = (0U),
    SLOPE_SERIES2D_OWN_ARRAY          = (1U),
    SLOPE_SERIES2D_DRAW_LINE          = (1U << 1U),
    SLOPE_SERIES2D_DRAW_MARKER        = (1U << 2U)
} SlopeSeries2DOption;


struct _SlopeSeries2D
{
    /*< parent object space >*/
    SlopePlot2D parent;

    /*< private >*/
    gpointer _slope_reserved1;
    gpointer _slope_reserved2;
    gpointer _slope_reserved3;
    gpointer _slope_reserved4;
};


struct _SlopeSeries2DClass
{
    SlopePlot2DClass parent_class;

    /* Padding for future expansion */
    void (*_slope_reserved1) (void);
    void (*_slope_reserved2) (void);
    void (*_slope_reserved3) (void);
    void (*_slope_reserved4) (void);
};


GType slope_series2d_get_type (void) G_GNUC_CONST;

SlopeItem* slope_series2d_new (void);

SlopeItem* slope_series2d_new_formatted (const char *format);

void slope_series2d_format (SlopeSeries2D *self, const char *format);

void slope_series2d_set_data (SlopeSeries2D *self, SlopeArray2D *array, gboolean own);

G_END_DECLS

#endif /* _slope_series2d_h_ */
