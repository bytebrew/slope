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

#ifndef SLOPE_XYSCALE_H
#define SLOPE_XYSCALE_H

#include <slope/scale.h>

#define SLOPE_XYSCALE_TYPE              (slope_xyscale_get_type())
#define SLOPE_XYSCALE(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_XYSCALE_TYPE, SlopeXyScale))
#define SLOPE_XYSCALE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_XYSCALE_TYPE, SlopeScaleXyClass))
#define SLOPE_IS_XYSCALE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_XYSCALE_TYPE))
#define SLOPE_IS_XYSCALE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_XYSCALE_TYPE))

SLOPE_BEGIN_DECLS

typedef struct
_SlopeXyScale
{
  SlopeScale parent;

  /* Padding to allow adding up to 4 members
     without breaking ABI. */
  gpointer padding[4];
}
SlopeXyScale;


typedef struct
_SlopeXyScaleClass
{
  SlopeScaleClass parent_class;

}
SlopeXyScaleClass;



GType slope_scale_get_type (void) G_GNUC_CONST;

SlopeScale* slope_xyscale_new ();

SLOPE_END_DECLS

#endif /* SLOPE_XYSCALE_H */
