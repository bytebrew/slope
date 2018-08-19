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

#ifndef _slope_plot2d_h_
#define _slope_plot2d_h_

#include "slope/item.h"

G_BEGIN_DECLS


#define SLOPE_TYPE_PLOT2D                 (slope_plot2d_get_type ())
#define SLOPE_PLOT2D(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_PLOT2D, SlopePlot2D))
#define SLOPE_PLOT2D_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_PLOT2D, SlopePlot2DClass))
#define SLOPE_IS_PLOT2D(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_PLOT2D))
#define SLOPE_IS_PLOT2D_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_PLOT2D))
#define SLOPE_PLOT2D_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_PLOT2D, SlopePlot2DClass))

typedef struct _SlopePlot2D             SlopePlot2D;
typedef struct _SlopePlot2DClass        SlopePlot2DClass;


struct _SlopePlot2D
{
    /*< parent object space >*/
    SlopeItem parent;

    /*< private >*/
    gpointer _slope_reserved1;
    gpointer _slope_reserved2;
    gpointer _slope_reserved3;
    gpointer _slope_reserved4;
};


struct _SlopePlot2DClass
{
    SlopeItemClass parent_class;


    void (*get_data_extents) (SlopePlot2D *self,
                              double *x_min, double *x_max,
                              double *y_min, double *y_max);

    /* Padding for future expansion */
    void (*_slope_reserved1) (void);
    void (*_slope_reserved2) (void);
    void (*_slope_reserved3) (void);
    void (*_slope_reserved4) (void);
};


GType slope_plot2d_get_type (void) G_GNUC_CONST;

void slope_plot2d_get_data_extents (SlopePlot2D *self,
                                    double *x_min, double *x_max,
                                    double *y_min, double *y_max);

G_END_DECLS

#endif /* _slope_plot2d_h_ */
