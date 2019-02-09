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

#ifndef _slope_view_h_
#define _slope_view_h_

#include <gtk/gtk.h>
#include "slope/figure.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_VIEW                 (slope_view_get_type ())
#define SLOPE_VIEW(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_VIEW, SlopeView))
#define SLOPE_VIEW_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_VIEW, SlopeViewClass))
#define SLOPE_IS_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_VIEW))
#define SLOPE_IS_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_VIEW))
#define SLOPE_VIEW_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_VIEW, SlopeViewClass))

typedef struct _SlopeView             SlopeView;
typedef struct _SlopeViewClass        SlopeViewClass;


struct _SlopeView
{
  /*< parent object space >*/
  GtkDrawingArea parent;

  /*< private >*/
  gpointer _slope_reserved1;
  gpointer _slope_reserved2;
  gpointer _slope_reserved3;
  gpointer _slope_reserved4;
};


struct _SlopeViewClass
{
  GtkDrawingAreaClass parent_class;

  /* Padding for future expansion */
  void (*_slope_reserved1) (void);
  void (*_slope_reserved2) (void);
  void (*_slope_reserved3) (void);
  void (*_slope_reserved4) (void);
};


GDK_AVAILABLE_IN_ALL
GType slope_view_get_type (void) G_GNUC_CONST;

GDK_AVAILABLE_IN_ALL
GtkWidget* slope_view_new (void);

GDK_AVAILABLE_IN_ALL
void slope_view_set_figure (SlopeView *self, SlopeFigure *figure);

GDK_AVAILABLE_IN_ALL
SlopeFigure* slope_view_get_figure (SlopeView *self);

GDK_AVAILABLE_IN_ALL
void slope_view_redraw (SlopeView *self);

G_END_DECLS

#endif /* _slope_view_h_ */
