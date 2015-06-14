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

#ifndef SLOPE_VIEW_H
#define SLOPE_VIEW_H

#include "slope/figure.h"
#include "slope/metrics.h"
#include "slope/item.h"

#include <glib-object.h>
#include <gtk/gtk.h>

SLOPE_BEGIN_DECLS

#define SLOPE_VIEW_TYPE            (slope_view_get_type())
#define SLOPE_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_VIEW_TYPE, SlopeView))
#define SLOPE_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_VIEW_TYPE, SlopeViewClass))
#define SLOPE_IS_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_VIEW_TYPE))
#define SLOPE_IS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_VIEW_TYPE))


/**
 */
typedef struct _SlopeView SlopeView;


/**
 */
typedef struct _SlopeViewClass SlopeViewClass;


/**
 */
struct _SlopeView
{
    GtkDrawingArea parent;
};


/**
 */
struct _SlopeViewClass
{
    GtkDrawingAreaClass parent;
};


/**
 */
slope_public GType
slope_view_get_type(void) G_GNUC_CONST;


/**
 */
slope_public GtkWidget* slope_view_new (void);


/**
 */
slope_public GtkWidget*
slope_view_new_for_figure (slope_figure_t *figure);

/**
 */
slope_public void
slope_view_toggle_mouse_zoom (GtkWidget *view,
                              slope_bool_t on);

SLOPE_END_DECLS

#endif /* SLOPE_VIEW_H */
