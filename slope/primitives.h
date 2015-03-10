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


/**
 * slope's public headers doesn't include cairo
 * so we define this symbol here to use in function
 * declarations
 */
typedef struct _cairo cairo_t;


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

typedef enum
{
    SLOPE_BLACK,
    SLOPE_WHITE,
    SLOPE_RED,
    SLOPE_GREEN,
    SLOPE_BLUE
}
slope_color_name_t;

typedef enum
{
    SLOPE_LINE,
    SLOPE_CIRCLES
}
slope_scatter_t;

typedef struct
{
    double red;
    double green;
    double blue;
    double alpha;
}
slope_color_t;

/**
 * Sets the coordinates of a rectangle
 * @param rect the target rectangle
 * @param x the x coordinate of the top left corner
 * @param y the y coordinate of the top left corner
 * @param width the width of rectangle
 * @param height the height of the rectangle
 */
void slope_rect_set (slope_rect_t *rect, double x,
                     double y, double w, double h);

/**
 */
void slope_color_set (slope_color_t *color, double r,
                      double g, double b, double a);

/**
 */
void slope_color_set_name (slope_color_t *color,
                           slope_color_name_t name);

#ifdef __cplusplus
}
#endif

#endif /*_SLOPE_PRIMITIVES_H_*/
