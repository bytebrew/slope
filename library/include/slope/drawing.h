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

#ifndef _slope_drawing_h_
#define _slope_drawing_h_

#include "slope/color.h"
#include <cairo.h>

#define SLOPE_ROUND_COORD(X) (((double) ((long) (X))) + 0.5)

G_BEGIN_DECLS

typedef struct
_SlopePoint {
    double x;
    double y;
} SlopePoint;


typedef struct
_SlopeRect {
    double x;
    double y;
    double width;
    double height;
} SlopeRect;


#define slope_point_get_diff(RES,LHS,RHS) \
    G_STMT_START {\
        (RES).x = (LHS).x - (RHS).x; \
        (RES).y = (LHS).y - (RHS).y; \
    } G_STMT_END


#define slope_point_get_sum(RES,LHS,RHS) \
    G_STMT_START {\
        (RES).x = (LHS).x + (RHS).x; \
        (RES).y = (LHS).y + (RHS).y; \
    } G_STMT_END


#define slope_point_get_ortogonal(R,P) \
    G_STMT_START {\
        (R).x = (P).y; \
        (R).y = -(P).x; \
    } G_STMT_END


#define slope_point_dotprod(LHS,RHS) \
    ((LHS).x * (RHS).x + (LHS).y * (RHS).y)


#define slope_point_length(V) \
    sqrt(slope_point_dotprod(V,V))


#define slope_point_get_normalized(N,V) \
    G_STMT_START {\
        double len = slope_point_length(V); \
        (N).x = (V).x / len; \
        (N).y = (V).y / len; \
    } G_STMT_END


#define slope_move(Pt, Dist, Direc) \
    G_STMT_START {\
        (Pt).x += (Dist) * (Direc).x; \
        (Pt).y += (Dist) * (Direc).y; \
    } G_STMT_END


void slope_cairo_set_rgba (cairo_t *cr, SlopeRGBA color);

void slope_cairo_rect (cairo_t *cr, const SlopeRect *rect);

void slope_cairo_round_rect (cairo_t *cr, const SlopeRect *rec, double radius);

void slope_cairo_draw (cairo_t *cr, double stroke_width, SlopeRGBA fill_color, SlopeRGBA stroke_color);

G_END_DECLS

#endif /* _slope_drawing_h_ */
