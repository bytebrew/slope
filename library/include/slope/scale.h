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

#ifndef _slope_scale_h_
#define _slope_scale_h_

#include "slope/item.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_SCALE                 (slope_scale_get_type ())
#define SLOPE_SCALE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_SCALE, SlopeScale))
#define SLOPE_SCALE_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_SCALE, SlopeScaleClass))
#define SLOPE_IS_SCALE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_SCALE))
#define SLOPE_IS_SCALE_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_SCALE))
#define SLOPE_SCALE_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_SCALE, SlopeScaleClass))

typedef struct _SlopeScale             SlopeScale;
typedef struct _SlopeScaleClass        SlopeScaleClass;


typedef enum _SlopeScaleTrait {
    SLOPE_SCALE_REVERSE_TICKS = (1U),
    SLOPE_SCALE_ANTIALIAS     = (1U << 1UL),
} SlopeScaleTrait;


struct _SlopeScale
{
    /*< parent object space >*/
    SlopeItem parent;

    /*< private >*/
    gpointer _slope_reserved1;
    gpointer _slope_reserved2;
    gpointer _slope_reserved3;
    gpointer _slope_reserved4;
};


struct _SlopeScaleClass
{
    SlopeItemClass parent_class;

    /* Padding for future expansion */
    void (*_slope_reserved1) (void);
    void (*_slope_reserved2) (void);
    void (*_slope_reserved3) (void);
    void (*_slope_reserved4) (void);
};


GType slope_scale_get_type (void) G_GNUC_CONST;

SlopeItem* slope_scale_new (SlopeScaleTrait traits);

void slope_scale_set_trait (SlopeScale *self, SlopeScaleTrait trait, gboolean toggle);

SlopeScaleTrait slope_scale_get_traits (SlopeScale *self);

void slope_scale_set_figure_position (SlopeScale *self, const SlopePoint *p1, const SlopePoint *p2);

void slope_scale_set_data_extents (SlopeScale *self, double min, double max);

G_END_DECLS

#endif /* _slope_axis2d_h_ */
