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

#ifndef _slope_grid_h_
#define _slope_grid_h_

#include "slope/frame.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_GRID                 (slope_grid_get_type ())
#define SLOPE_GRID(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_GRID, SlopeGrid))
#define SLOPE_GRID_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_GRID, SlopeGridClass))
#define SLOPE_IS_GRID(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_GRID))
#define SLOPE_IS_GRID_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_GRID))
#define SLOPE_GRID_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_GRID, SlopeGridClass))

typedef struct _SlopeGrid             SlopeGrid;
typedef struct _SlopeGridClass        SlopeGridClass;


struct _SlopeGrid
{
  /*< parent object space >*/
  SlopeFrame parent;

  /*< private >*/
  gpointer _slope_reserved1;
  gpointer _slope_reserved2;
  gpointer _slope_reserved3;
  gpointer _slope_reserved4;
};


struct _SlopeGridClass
{
  SlopeItemClass parent_class;

  /* Padding for future expansion */
  void (*_slope_reserved1) (void);
  void (*_slope_reserved2) (void);
  void (*_slope_reserved3) (void);
  void (*_slope_reserved4) (void);
};


GType slope_grid_get_type (void) G_GNUC_CONST;

SlopeItem* slope_grid_new (void);

void slope_grid_emplace (SlopeGrid *self, SlopeItem *child,
                         int x, int y, int width, int height);

void slope_grid_set_child_position (SlopeGrid *self, SlopeItem *child,
                                    int x, int y, int width, int height);

G_END_DECLS

#endif /* _slope_grid_h_ */
