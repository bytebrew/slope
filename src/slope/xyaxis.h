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

#ifndef SLOPE_XYAXIS_H
#define SLOPE_XYAXIS_H

#include <slope/item.h>

#define SLOPE_XYAXIS_TYPE              (slope_xyaxis_get_type())
#define SLOPE_XYAXIS(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SLOPE_XYAXIS_TYPE, SlopeXyAxis))
#define SLOPE_XYAXIS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SLOPE_XYAXIS_TYPE, SlopeXyAxisClass))
#define SLOPE_IS_XYAXIS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SLOPE_XYAXIS_TYPE))
#define SLOPE_IS_XYAXIS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), SLOPE_XYAXIS_TYPE))
#define SLOPE_XYAXIS_GET_CLASS(obj)    (SLOPE_XYAXIS_CLASS(G_OBJECT_GET_CLASS(obj)))

SLOPE_BEGIN_DECLS

typedef enum
_SlopeXyAxisMode
{
    SLOPE_XYAXIS_VERTICAL           = 0x00000001,
    SLOPE_XYAXIS_HORIZONTAL         = 0x00000002
}
SlopeXyAxisMode;


typedef struct
_SlopeXyAxis
{
    SlopeItem parent;
}
SlopeXyAxis;


typedef struct
_SlopeXyAxisClass
{
    SlopeItem parent_klass;
}
SlopeXyAxisClass;


GType slope_xyaxis_get_type (void) G_GNUC_CONST;

SlopeXyAxis* slope_xyaxis_new (int mode);

SLOPE_END_DECLS

#endif /* SLOPE_XYAXIS_H */
