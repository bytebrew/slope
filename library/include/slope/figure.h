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

#ifndef _slope_figure_h_
#define _slope_figure_h_

#include <glib-object.h>
#include <cairo.h>
#include "slope/drawing.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_FIGURE                 (slope_figure_get_type ())
#define SLOPE_FIGURE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_FIGURE, SlopeFigure))
#define SLOPE_FIGURE_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_FIGURE, SlopeFigureClass))
#define SLOPE_IS_FIGURE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_FIGURE))
#define SLOPE_IS_FIGURE_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_FIGURE))
#define SLOPE_FIGURE_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_FIGURE, SlopeFigureClass))

typedef struct _SlopeFigure             SlopeFigure;
typedef struct _SlopeFigureClass        SlopeFigureClass;


struct _SlopeFigure
{
  /*< parent object space >*/
  GObject parent;

  /*< private >*/
  gpointer dummy;
};


struct _SlopeFigureClass
{
  GObjectClass parent_class;

  void (*draw) (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);

  /* Padding for future expansion */
  void (*_slope_reserved1) (void);
  void (*_slope_reserved2) (void);
  void (*_slope_reserved3) (void);
  void (*_slope_reserved4) (void);
};


GType slope_figure_get_type (void) G_GNUC_CONST;

SlopeFigure* slope_figure_new (void);

void slope_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);

G_END_DECLS

#endif /* _slope_figure_h_ */
