/*
 * Copyright (C) 2017  Elvis Teixeira
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

#ifndef SLOPE_CHART_H
#define SLOPE_CHART_H

#include <slope/view.h>

#define SLOPE_CHART_TYPE (slope_chart_get_type())
#define SLOPE_CHART(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_CHART_TYPE, SlopeChart))
#define SLOPE_CHART_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_CHART_TYPE, SlopeChartClass))
#define SLOPE_IS_CHART(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_CHART_TYPE))
#define SLOPE_IS_CHART_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_CHART_TYPE))
#define SLOPE_CHART_GET_CLASS(obj) (SLOPE_CHART_CLASS(G_OBJECT_GET_CLASS(obj)))

SLOPE_BEGIN_DECLS

typedef struct _SlopeChart
{
  GtkWindow parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
} SlopeChart;

typedef struct _SlopeChartClass
{
  GtkWindowClass parent_class;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
} SlopeChartClass;

GType slope_chart_get_type(void) G_GNUC_CONST;

GtkWidget *slope_chart_new();

GtkWidget *slope_chart_new_detailed(const gchar *title, int width, int height);

SlopeFigure *slope_chart_get_figure(SlopeChart *self);

GtkWidget *slope_chart_get_header(SlopeChart *self);

GtkWidget *slope_chart_get_view(SlopeChart *self);

void slope_chart_redraw(SlopeChart *self);

void slope_chart_add_scale(SlopeChart *self, SlopeScale *scale);

void slope_chart_write_to_png(SlopeChart *self,
                              const char *filename,
                              int         width,
                              int         height);

SLOPE_END_DECLS

#endif /* SLOPE_CHART_H */
