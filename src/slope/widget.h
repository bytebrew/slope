/*
 * Copyright (C) 2015  Elvis Teixeira
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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SLOPE_WIDGET__
#define __SLOPE_WIDGET__

#include "slope/figure.h"
#include "slope/linear.h"
#include "slope/series.h"

#include <glib-object.h>
#include <gtk/gtk.h>

SLOPE_BEGIN_DECLS

#define SLOPE_WIDGET_TYPE              (slope_widget_get_type())
#define SLOPE_WIDGET(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_WIDGET_TYPE, SlopeWidget))
#define SLOPE_WIDGET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_WIDGET_TYPE, SlopeWidgetClass))
#define SLOPE_IS_WIDGET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_WIDGET_TYPE))
#define SLOPE_IS_WIDGET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_WIDGET_TYPE))

typedef struct _SlopeWidget SlopeWidget;
typedef struct _SlopeWidgetClass SlopeWidgetClass;

struct _SlopeWidget
{
    GtkDrawingArea parent;
};

struct _SlopeWidgetClass
{
    GtkDrawingAreaClass parent;
    
    /* padding for future expansion */
    gpointer padding;
};

GType slope_widget_get_type(void) G_GNUC_CONST;

GtkWidget* slope_widget_new (void);

GtkWidget* slope_widget_new_for_figure (slope_figure_t *figure);

slope_figure_t* slope_widget_get_figure (GtkWidget *widget);

SLOPE_END_DECLS

#endif /* __SLOPE_WIDGET__ */
