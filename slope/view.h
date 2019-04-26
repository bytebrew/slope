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

#ifndef SLOPE_VIEW_H
#define SLOPE_VIEW_H

#if !defined (SLOPE_H_INSIDE) && !defined (SLOPE_COMPILATION)
#error "Only <slope.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <slope/figure.h>

G_BEGIN_DECLS

#define SLOPE_TYPE_VIEW slope_view_get_type ()
G_DECLARE_DERIVABLE_TYPE (SlopeView, slope_view, SLOPE, VIEW, GtkDrawingArea)

struct _SlopeViewClass
{
  GtkDrawingAreaClass parent_class;

  /* Padding to allow adding up to 16 new virtual
   * functions without breaking ABI. */
  gpointer padding[16];
};


GtkWidget *slope_view_new (void);

SlopeFigure *slope_view_get_figure (SlopeView *self);

void slope_view_set_figure (SlopeView *self, SlopeFigure *figure, gboolean own);

G_END_DECLS

#endif /* SLOPE_VIEW_H */
