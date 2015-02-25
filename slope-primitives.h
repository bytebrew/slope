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

#ifndef _SLOPE_PRIMITIVES_H_
#define _SLOPE_PRIMITIVES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    double x;
    double y;
}
slope_point_t;

typedef struct
{
    double x;
    double y;
    double width;
    double height;
}
slope_rect_t;

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_PRIMITIVES_H_*/
