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

#include <slope/boxplot.h>
#include <slope/scale.h>


typedef struct
_SlopeBoxplotPrivate
{
    SlopeColor line_color;
}
SlopeBoxplotPrivate;


#define SLOPE_BOXPLOT_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_BOXPLOT_TYPE, SlopeBoxplotPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeBoxplot, slope_boxplot, SLOPE_ITEM_TYPE)

static void _boxplot_finalize (GObject *self);

static void
slope_boxplot_class_init (SlopeBoxplotClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    SlopeItemClass *item_klass = SLOPE_ITEM_CLASS(klass);
}

static void
slope_boxplot_init (SlopeBoxplot *self) {
    SlopeBoxplotPrivate *priv = SLOPE_BOXPLOT_GET_PRIVATE(self);
}

static void
_boxplot_finalize (GObject *self) {
    G_OBJECT_CLASS(slope_boxplot_parent_class)->finalize(self);
}

SlopeItem* slope_boxplot_new(void) {
    SlopeItem *self = SLOPE_ITEM(g_object_new(SLOPE_BOXPLOT_TYPE, NULL));
    return self;
}

/* slope/boxplot.c */

