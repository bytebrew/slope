/*
 * Copyright (C) 2016  Elvis Teixeira
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

typedef struct
_SlopeChartPrivate
{
    GtkWidget *header;
    GtkWidget *view;
    SlopeFigure *figure;
}
SlopeChartPrivate;


#define SLOPE_CHART_GET_CLASS(obj) \
    (SLOPE_CHART_CLASS(G_OBJECT_GET_CLASS(obj)))

#define SLOPE_CHART_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_CHART_TYPE, SlopeChartPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeChart, slope_chart, GTK_TYPE_WINDOW)



static void _chart_finalize (GObject *self);



static void
slope_chart_class_init (SlopeChartClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _chart_finalize;
}


static
void _chart_finalize (GObject *self)
{
    SLOPE_UNUSED(self);
    // TODO
}


static void
slope_chart_init (SlopeChart *self)
{
    SlopeChartPrivate *priv = SLOPE_CHART_GET_PRIVATE(self);

    priv->header = gtk_header_bar_new();
    priv->figure = slope_figure_new();
    priv->view = slope_view_new_with_figure(priv->figure);

    gtk_window_set_default_size(GTK_WINDOW(self), 600, 600);

    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(priv->header), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(self), priv->header);

    gtk_container_add(GTK_CONTAINER(self), priv->view);
}


GtkWidget* slope_chart_new ()
{
    GtkWidget *self = GTK_WIDGET(g_object_new(SLOPE_CHART_TYPE, NULL));

    return self;
}

/* slope/chart.c */
