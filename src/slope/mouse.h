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

#ifndef SLOPE_MOUSE_H
#define SLOPE_MOUSE_H

#include <slope/global.h>

SLOPE_BEGIN_DECLS

#define SLOPE_MOUSE_LEFT_CLICK      0x00000001
#define SLOPE_MOUSE_RIGHT_CLICK     0x00000002
#define SLOPE_MOUSE_LEFT_RELEASE    0x00000003
#define SLOPE_MOUSE_RIGHT_RELEASE   0x00000004
#define SLOPE_MOUSE_HOVER           0x00000005


typedef struct
_SlopeMouseEvent
{
    double x;
    double y;
    unsigned int type;
}
SlopeMouseEvent;


SLOPE_END_DECLS

#endif /* SLOPE_MOUSE_H */
