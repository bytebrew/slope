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

#ifndef SLOPE_DRAWING_H
#define SLOPE_DRAWING_H

#include <cairo/cairo.h>
#include <glib.h>
#include <slope/global.h>

SLOPE_BEGIN_DECLS

typedef guint32 SlopeColor;
#define SLOPE_WHITE 0xFFFFFFFF
#define SLOPE_RED 0xFF0000FF
#define SLOPE_GREEN 0x00FF00FF
#define SLOPE_BLUE 0x0000FFFF
#define SLOPE_BLACK 0x000000FF
#define SLOPE_COLOR_NULL 0x00000000
#define SLOPE_YELLOW 0xFFFF00FF
#define SLOPE_CYAN 0x00FFFFFF
#define SLOPE_BLUEVIOLET 0x8A2BE2FF
#define SLOPE_BROWN 0xA52A2AFF
#define SLOPE_CADETBLUE 0x5F9EA0FF
#define SLOPE_CHOCOLATE 0xD2691EFF
#define SLOPE_DARKGOLDENROD 0xB8860BFF
#define SLOPE_DARKMAGENTA 0x8B008BFF
#define SLOPE_HOTPINK 0xFF69B4FF
#define SLOPE_LIGHTSKYBLUE 0x87CEFAFF
#define SLOPE_LIGHTSTEELBLUE 0xB0C4DEFF
#define SLOPE_MIDNIGHTBLUE 0x191970FF
#define SLOPE_MAROON 0x800000FF
#define SLOPE_TEAL 0x008080FF
#define SLOPE_PALETURQUOISE 0xAFEEEEFF

#define SLOPE_GET_RED(color) (((color) >> 24) & 0xFF)
#define SLOPE_GET_GREEN(color) (((color) >> 16) & 0xFF)
#define SLOPE_GET_BLUE(color) (((color) >> 8) & 0xFF)
#define SLOPE_GET_ALPHA(color) ((color) &0xFF)
#define SLOPE_GET_REDF(color) (((double) SLOPE_GET_RED(color)) / 255.0)
#define SLOPE_GET_GREENF(color) (((double) SLOPE_GET_GREEN(color)) / 255.0)
#define SLOPE_GET_BLUEF(color) (((double) SLOPE_GET_BLUE(color)) / 255.0)
#define SLOPE_GET_ALPHAF(color) (((double) SLOPE_GET_ALPHA(color)) / 255.0)
#define SLOPE_SET_RED(color, red) \
  color = (((color) &0x00FFFFFF) | (((red) &0xFF) << 24))
#define SLOPE_SET_GREEN(color, green) \
  color = (((color) &0xFF00FFFF) | (((green) &0xFF) << 16))
#define SLOPE_SET_BLUE(color, blue) \
  color = (((color) &0xFFFF00FF) | (((blue) &0xFF) << 8))
#define SLOPE_SET_ALPHA(color, alpha) \
  color = (((color) &0xFFFFFF00) | ((alpha) &0xFF))
#define SLOPE_COLOR(r, g, b, a) \
  ((((r) &0xFF) << 24) | (((g) &0xFF) << 16) | (((b) &0xFF) << 8) | ((a) &0xFF))
#define SLOPE_COLOR_IS_NULL(color) (SLOPE_GET_ALPHA(color) == 0)
#define SLOPE_COORD_TO_PIXEL(coord) (((int) coord) + 0.5)
#define SLOPE_GRAY(value) SLOPE_COLOR(value, value, value, 0xFF)

#define SLOPE_ABS(a) (((a) < 0) ? -(a) : (a))
#define SLOPE_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define SLOPE_MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct _SlopePoint
{
  double x;
  double y;
} SlopePoint;

typedef struct _SlopeRect
{
  double x;
  double y;
  double width;
  double height;
} SlopeRect;

typedef enum _SlopeOrientation {
  SLOPE_VERTICAL,
  SLOPE_HORIZONTAL
} SlopeOrientation;

typedef enum _SlopeCorner {
  SLOPE_TOPLEFT,
  SLOPE_TOPRIGHT,
  SLOPE_BOTTOMLEFT,
  SLOPE_BOTTOMRIGHT
} SlopeCorner;

gboolean slope_similar(double x1, double x2);

gboolean slope_rect_contains(const SlopeRect *rect, double x, double y);

void slope_rect_translate(SlopeRect *rect, double dx, double dy);

void slope_cairo_set_color(cairo_t *cr, SlopeColor color);

void slope_cairo_set_antialias(cairo_t *cr, gboolean antialias);

void slope_cairo_line(cairo_t *cr, const SlopePoint *p1, const SlopePoint *p2);

void slope_cairo_line_cosmetic(cairo_t *         cr,
                               const SlopePoint *p1,
                               const SlopePoint *p2,
                               double            width);

void slope_cairo_rect(cairo_t *cr, const SlopeRect *rec);

void slope_cairo_round_rect(cairo_t *cr, const SlopeRect *rec, double rad);

void slope_cairo_draw(cairo_t *cr, SlopeColor stroke, SlopeColor fill);

void slope_cairo_text(cairo_t *cr, double x, double y, const char *utf8);

void slope_cairo_circle(cairo_t *cr, const SlopePoint *center, double radius);

SlopeColor slope_color_parse(char c);

SLOPE_END_DECLS

#endif /* SLOPE_DRAWING_H */
