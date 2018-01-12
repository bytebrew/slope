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

#include <slope/chart.h>

typedef struct _SlopeChartPrivate
{
  GtkWidget *  header;
  GtkWidget *  view;
  SlopeFigure *figure;
} SlopeChartPrivate;

#define SLOPE_CHART_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_CHART_TYPE, SlopeChartPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(SlopeChart, slope_chart, GTK_TYPE_WINDOW)

static void _chart_finalize(GObject *self);

static void slope_chart_class_init(SlopeChartClass *klass)
{
  GObjectClass *object_klass = G_OBJECT_CLASS(klass);
  object_klass->finalize     = _chart_finalize;
}

static void slope_chart_init(SlopeChart *self)
{
  SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self);

  priv->header = gtk_header_bar_new();
  priv->figure = slope_figure_new();
  priv->view   = slope_view_new_with_figure(priv->figure);
  gtk_window_set_default_size(GTK_WINDOW(self), 530, 500);

  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(priv->header), TRUE);
  gtk_header_bar_set_title(GTK_HEADER_BAR(priv->header), "Slope");
  gtk_header_bar_set_subtitle(GTK_HEADER_BAR(priv->header), "Gtk Chart");
  gtk_window_set_titlebar(GTK_WINDOW(self), priv->header);

  gtk_container_add(GTK_CONTAINER(self), priv->view);
}

static void _chart_finalize(GObject *self)
{
  /* SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self); */
  G_OBJECT_CLASS(slope_chart_parent_class)->finalize(self);
}

GtkWidget *slope_chart_new()
{
  GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_CHART_TYPE, NULL));
  return self;
}

GtkWidget *slope_chart_new_detailed(const gchar *title, int width, int height)
{
  GtkWidget *        self = GTK_WIDGET(g_object_new(SLOPE_CHART_TYPE, NULL));
  SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self);
  gtk_window_set_default_size(GTK_WINDOW(self), width, height);
  gtk_header_bar_set_title(GTK_HEADER_BAR(priv->header), title);
  return self;
}

void slope_chart_add_scale(SlopeChart *self, SlopeScale *scale)
{
  SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self);
  slope_figure_add_scale(priv->figure, scale);
}

void slope_chart_write_to_png(SlopeChart *self,
                              const char *filename,
                              int         width,
                              int         height)
{
  SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self);
  slope_figure_write_to_png(priv->figure, filename, width, height);
}

void slope_chart_redraw(SlopeChart *self)
{
  SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self);
  slope_view_redraw(SLOPE_VIEW(priv->view));
}

SlopeFigure *slope_chart_get_figure(SlopeChart *self)
{
  return SLOPE_CHART_GET_PRIVATE(self)->figure;
}

GtkWidget *slope_chart_get_header(SlopeChart *self)
{
  return SLOPE_CHART_GET_PRIVATE(self)->header;
}

GtkWidget *slope_chart_get_view(SlopeChart *self)
{
  return SLOPE_CHART_GET_PRIVATE(self)->view;
}

/* slope/chart.c */
