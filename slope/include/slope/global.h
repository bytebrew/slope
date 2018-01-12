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

#ifndef SLOPE_GLOBAL_H
#define SLOPE_GLOBAL_H

#ifdef __cplusplus
#define SLOPE_BEGIN_DECLS extern "C" {
#define SLOPE_END_DECLS } /* extern "C" */
#else
#define SLOPE_BEGIN_DECLS /* nothing */
#define SLOPE_END_DECLS   /* nothing */
#endif

typedef struct _SlopeFigure SlopeFigure;
typedef struct _SlopeScale  SlopeScale;
typedef struct _SlopeItem   SlopeItem;
typedef struct _SlopeView   SlopeView;

typedef enum _SlopeMouseButton {
  SLOPE_MOUSE_BUTTON_LEFT,
  SLOPE_MOUSE_BUTTON_RIGHT,
  SLOPE_MOUSE_BUTTON_NONE
} SlopeMouseButton;

typedef enum _SlopeMouseEventType {
  SLOPE_MOUSE_PRESS,
  SLOPE_MOUSE_DOUBLE_PRESS,
  SLOPE_MOUSE_MOVE,
  SLOPE_MOUSE_MOVE_PRESSED,
  SLOPE_MOUSE_RELEASE
} SlopeMouseEventType;

typedef struct _SlopeMouseEvent
{
  double              x;
  double              y;
  SlopeMouseButton    button;
  SlopeMouseEventType type;
} SlopeMouseEvent;

#define SLOPE_UNUSED(var) (void) var

#endif /* SLOPE_GLOBAL_H */
