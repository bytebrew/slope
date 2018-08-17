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

#ifndef _slope_axis2d_h_
#define _slope_axis2d_h_

#include "slope/item.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_AXIS2D                 (slope_axis2d_get_type ())
#define SLOPE_AXIS2D(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_AXIS2D, SlopeAxis2D))
#define SLOPE_AXIS2D_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_AXIS2D, SlopeAxia2DClass))
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


struct _SlopeAxis2D
{
  /*< parent object space >*/
  SlopeItem parent;

  /*< private >*/
  gpointer dummy;
};


struct _SlopeAxis2DClass
{
  SlopeItemClass parent_class;

  /* Padding for future expansion */
  void (*_slope_reserved1) (void);
  void (*_slope_reserved2) (void);
  void (*_slope_reserved3) (void);
  void (*_slope_reserved4) (void);
};


GType slope_axis2d_get_type (void) G_GNUC_CONST;

SlopeItem* slope_axis2d_new (void);

G_END_DECLS

#endif /* _slope_axis2d_h_ */
