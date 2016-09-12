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

#include <slope/scale.h>

typedef struct
_SlopeScalePrivate
{
    int pad;
}
SlopeScalePrivate;


#define SLOPE_SCALE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_SCALE_TYPE, SlopeScalePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeScale,
    slope_scale,
    SLOPE_ITEM_TYPE)


static void _scale_finalize (GObject *self);
static void _scale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
static void _scale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src);
static void _scale_rescale (SlopeScale *self);


static void
slope_scale_class_init (SlopeScaleClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _scale_finalize;

    klass->map = _scale_map;
    klass->unmap = _scale_unmap;
    klass->rescale = _scale_rescale;
}


static void
slope_scale_init (SlopeScale *self)
{

}


static
void _scale_finalize (GObject *self)
{
    GObjectClass *parent_class = g_type_class_peek_parent(G_OBJECT_GET_CLASS(self));

    /* just chain up */
    G_OBJECT_CLASS(parent_class)->finalize(self);
}


SlopeScale* slope_scale_new (void)
{
    SlopeScale *self = SLOPE_SCALE(g_object_new(SLOPE_SCALE_TYPE, NULL));

    return self;
}


static
void _scale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{

}


static
void _scale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{

}


static
void _scale_rescale (SlopeScale *self)
{

}

void slope_scale_map (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SLOPE_SCALE_GET_CLASS(self)->map(self, res, src);
}


void slope_scale_unmap (SlopeScale *self, SlopePoint *res, const SlopePoint *src)
{
    SLOPE_SCALE_GET_CLASS(self)->unmap(self, res, src);
}


void slope_scale_rescale (SlopeScale *self)
{
    SLOPE_SCALE_GET_CLASS(self)->rescale(self);
}

/* slope/scale.c */
