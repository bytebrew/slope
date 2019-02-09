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

#ifndef _slope_axis2d_h_
#define _slope_axis2d_h_

#include "slope/frame.h"
#include "slope/plot2d.h"
#include "slope/scale.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_AXIS2D                 (slope_axis2d_get_type ())
#define SLOPE_AXIS2D(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_AXIS2D, SlopeAxis2D))
#define SLOPE_AXIS2D_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_AXIS2D, SlopeAxis2DClass))
#define SLOPE_IS_AXIS2D(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_AXIS2D))
#define SLOPE_IS_AXIS2D_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_AXIS2D))
#define SLOPE_AXIS2D_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_AXIS2D, SlopeAxis2DClass))

typedef struct _SlopeAxis2D             SlopeAxis2D;
typedef struct _SlopeAxis2DClass        SlopeAxis2DClass;


typedef enum {
    SLOPE_AXIS2D_TOP        = 0,
    SLOPE_AXIS2D_LEFT       = 1,
    SLOPE_AXIS2D_BOTTOM     = 2,
    SLOPE_AXIS2D_RIGHT      = 3,
    SLOPE_AXIS2D_X          = 4,
    SLOPE_AXIS2D_Y          = 5,
} SlopeAxisID;


typedef enum {
    SLOPE_AXIS2D_SCALE_BOTTOM     = (1U),
    SLOPE_AXIS2D_SCALE_LEFT       = (1U << 1U),
    SLOPE_AXIS2D_SCALE_TOP        = (1U << 2U),
    SLOPE_AXIS2D_SCALE_RIGHT      = (1U << 3U),
    SLOPE_AXIS2D_SCALE_X          = (1U << 4U),
    SLOPE_AXIS2D_SCALE_Y          = (1U << 5U),
} SlopeAxisScale;


typedef enum {
    SLOPE_AXIS2D_LINE_ANTIALIAS     = (1U),
    SLOPE_AXIS2D_DRAW_TICKS         = (1U << 1U)
} SlopeAxisOption;


struct _SlopeAxis2D
{
    /*< parent object space >*/
    SlopeFrame parent;

    /*< private >*/
    gpointer _slope_reserved1;
    gpointer _slope_reserved2;
    gpointer _slope_reserved3;
    gpointer _slope_reserved4;
};


struct _SlopeAxis2DClass
{
    SlopeFrameClass parent_class;

    void (*add_plot) (SlopeAxis2D *self, SlopePlot2D *plot);

    /* Padding for future expansion */
    void (*_slope_reserved1) (void);
    void (*_slope_reserved2) (void);
    void (*_slope_reserved3) (void);
    void (*_slope_reserved4) (void);
};


GType slope_axis2d_get_type (void) G_GNUC_CONST;

SlopeItem* slope_axis2d_new (const char *title);

SlopeScale* slope_axis2d_get_scale (SlopeAxis2D *self, SlopeAxisScale scale);

void slope_axis2d_set_scales (SlopeAxis2D *self, SlopeAxisScale scales);

void slope_axis2d_add_plot (SlopeAxis2D *self, SlopePlot2D *plot);

void slope_axis2d_update_scale (SlopeAxis2D *self);

void slope_axis2d_map (SlopeAxis2D *self, SlopePoint *f, const SlopePoint *d);

void slope_axis2d_unmap (SlopeAxis2D *self, SlopePoint *d, const SlopePoint *f);

G_END_DECLS

#endif /* _slope_axis2d_h_ */
