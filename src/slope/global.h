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

#ifndef SLOPE_GLOBAL_H
#define SLOPE_GLOBAL_H

#ifdef __cplusplus
# define SLOPE_BEGIN_DECLS extern "C" {
# define SLOPE_END_DECLS } /* extern "C" */
#else
# define SLOPE_BEGIN_DECLS /* nothing */
# define SLOPE_END_DECLS /* nothing */
#endif

typedef struct _SlopeFigure   SlopeFigure;
typedef struct _SlopeScale    SlopeScale;
typedef struct _SlopeItem     SlopeItem;
typedef struct _SlopeView     SlopeView;



typedef enum
_SlopeItemMouseButton
{
    SLOPE_VIEW_LEFT_BUTTON,
    SLOPE_VIEW_RIGHT_BUTTON,
    SLOPE_VIEW_NO_BUTTON
}
SlopeItemMouseButton;


typedef enum
_SlopeViewMouseEventType
{
    SLOPE_VIEW_BUTTON_PRESS,
    SLOPE_VIEW_BUTTON_DOUBLE_PRESS,
    SLOPE_VIEW_MOVE,
    SLOPE_VIEW_MOVE_PRESSED,
    SLOPE_VIEW_BUTTON_RELEASE
}
SlopeViewMouseEventType;


typedef struct
_SlopeViewMouseEvent
{
    double x;
    double y;
    int buttom;
    unsigned int type;
}
SlopeViewMouseEvent;

#define SLOPE_UNUSED(var) (void) var

#endif /* SLOPE_GLOBAL_H */
