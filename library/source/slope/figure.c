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

#include "slope/figure.h"

typedef struct _SlopeFigurePrivate SlopeFigurePrivate;
#define SLOPE_FIGURE_PRIVATE(Addr) ((SlopeFigurePrivate*) (Addr))

struct _SlopeFigurePrivate
{
    SlopeRGB background_color;
};


G_DEFINE_TYPE_WITH_PRIVATE (SlopeFigure, slope_figure, G_TYPE_OBJECT)
#define SLOPE_FIGURE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_TYPE_FIGURE, SlopeFigurePrivate))

/* local decls */
static void slope_figure_finalize(GObject *self);
static void slope_figure_dispose (GObject *self);


static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = slope_figure_dispose;
    gobject_class->finalize = slope_figure_finalize;



}

static void
slope_figure_init (SlopeFigure *self)
{
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    m->background_color = SlopeRGB_White;
}


static void
slope_figure_dispose (GObject *object)
{
  SlopeFigure *self = SLOPE_FIGURE (object);
  SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

  // TODO

  G_OBJECT_CLASS (slope_figure_parent_class)->dispose (object);
}


static void
slope_figure_finalize(GObject *object)
{
    SlopeFigure *self = SLOPE_FIGURE (object);
    SlopeFigurePrivate *m = SLOPE_FIGURE_GET_PRIVATE (self);

    // TODO

    G_OBJECT_CLASS (slope_figure_parent_class)->finalize (object);
}


SlopeFigure*
slope_figure_new (void)
{
    return SLOPE_FIGURE (g_object_new (SLOPE_TYPE_FIGURE, NULL));
}


/* slope/figure.c */
