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

#include "slope/view.h"
#include <stdio.h>

#define SLOPE_VIEW_PRIVATE(obj)          \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj),  \
    SLOPE_VIEW_TYPE, SlopeViewPrivate))


/**
*/
typedef struct _SlopeViewPrivate SlopeViewPrivate;


/**
 */
struct _SlopeViewPrivate
{
    slope_figure_t *figure;
};


G_DEFINE_TYPE(SlopeView, slope_view, GTK_TYPE_DRAWING_AREA);


static void
slope_view_class_init(SlopeViewClass *klass)
{
    g_type_class_add_private(klass, sizeof(SlopeViewPrivate));
}


static void
slope_view_init(SlopeView *view)
{
    SlopeViewPrivate *priv = SLOPE_VIEW_PRIVATE (view);
    priv->figure = slope_figure_create();
}


GtkWidget *
slope_view_new()
{
    return GTK_WIDGET(g_object_new(SLOPE_VIEW_TYPE, NULL));
}

/* slope/view.c */
