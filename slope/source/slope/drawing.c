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

#include <math.h>
#include <slope/drawing.h>

#define __SIMILAR_DOUBLE(x1, x2) ((fabs((x2) - (x1)) < 1e-4) ? TRUE : FALSE)

gboolean slope_similar(double x1, double x2)
{
  return __SIMILAR_DOUBLE(x1, x2);
}

gboolean slope_rect_contains(const SlopeRect *rect, double x, double y)
{
  if (x < rect->x || (rect->x + rect->width) <= x) return FALSE;
  if (y <= rect->y || (rect->y + rect->height) < y) return FALSE;
  return TRUE;
}

void slope_rect_translate(SlopeRect *rect, double dx, double dy)
{
  rect->x += dx;
  rect->y += dy;
}

void slope_cairo_set_color(cairo_t *cr, SlopeColor color)
{
  cairo_set_source_rgba(cr,
                        SLOPE_GET_REDF(color),
                        SLOPE_GET_GREENF(color),
                        SLOPE_GET_BLUEF(color),
                        SLOPE_GET_ALPHAF(color));
}

void slope_cairo_set_antialias(cairo_t *cr, gboolean antialias)
{
  cairo_set_antialias(
      cr, antialias == TRUE ? CAIRO_ANTIALIAS_SUBPIXEL : CAIRO_ANTIALIAS_NONE);
}

void slope_cairo_line(cairo_t *cr, const SlopePoint *p1, const SlopePoint *p2)
{
  cairo_move_to(cr, p1->x, p1->y);
  cairo_line_to(cr, p2->x, p2->y);
}

void slope_cairo_line_cosmetic(cairo_t *         cr,
                               const SlopePoint *p1,
                               const SlopePoint *p2,
                               double            width)
{
  double round_width = round(width);
  cairo_set_line_width(cr, round_width);
  if (__SIMILAR_DOUBLE(round_width, 1.0) ||
      __SIMILAR_DOUBLE(round_width, 3.0) || __SIMILAR_DOUBLE(round_width, 5.0))
    {
      cairo_move_to(
          cr, SLOPE_COORD_TO_PIXEL(p1->x), SLOPE_COORD_TO_PIXEL(p1->y));
      cairo_line_to(
          cr, SLOPE_COORD_TO_PIXEL(p2->x), SLOPE_COORD_TO_PIXEL(p2->y));
    }
  else if (__SIMILAR_DOUBLE(round_width, 2.0) ||
           __SIMILAR_DOUBLE(round_width, 4.0) ||
           __SIMILAR_DOUBLE(round_width, 6.0))
    {
      cairo_move_to(cr, round(p1->x), round(p1->y));
      cairo_line_to(cr, round(p2->x), round(p2->y));
    }
  else
    {
      cairo_move_to(cr, p1->x, p1->y);
      cairo_line_to(cr, p2->x, p2->y);
    }
}

void slope_cairo_rect(cairo_t *cr, const SlopeRect *r)
{
  cairo_rectangle(cr, r->x, r->y, r->width, r->height);
}

void slope_cairo_round_rect(cairo_t *cr, const SlopeRect *rec, double radius)
{
  const double degrees = 0.01745329252; /* ~pi/180 */
  cairo_new_sub_path(cr);
  cairo_arc(cr,
            rec->x + rec->width - radius,
            rec->y + radius,
            radius,
            -90 * degrees,
            0 * degrees);
  cairo_arc(cr,
            rec->x + rec->width - radius,
            rec->y + rec->height - radius,
            radius,
            0 * degrees,
            90 * degrees);
  cairo_arc(cr,
            rec->x + radius,
            rec->y + rec->height - radius,
            radius,
            90 * degrees,
            180 * degrees);
  cairo_arc(cr,
            rec->x + radius,
            rec->y + radius,
            radius,
            180 * degrees,
            270 * degrees);
  cairo_close_path(cr);
}

void slope_cairo_draw(cairo_t *cr, SlopeColor stroke, SlopeColor fill)
{
  if (!SLOPE_COLOR_IS_NULL(stroke) && !SLOPE_COLOR_IS_NULL(fill))
    {
      slope_cairo_set_color(cr, fill);
      cairo_fill_preserve(cr);
      slope_cairo_set_color(cr, stroke);
      cairo_stroke(cr);
    }
  else if (!SLOPE_COLOR_IS_NULL(stroke))
    {
      slope_cairo_set_color(cr, stroke);
      cairo_stroke(cr);
    }
  else
    {
      slope_cairo_set_color(cr, fill);
      cairo_fill(cr);
    }
}

void slope_cairo_text(cairo_t *cr, double x, double y, const char *utf8)
{
  cairo_move_to(cr, x, y);
  cairo_show_text(cr, utf8);
}

void slope_cairo_circle(cairo_t *cr, const SlopePoint *center, double radius)
{
  cairo_move_to(cr, center->x + radius, center->y);
  cairo_arc(cr, center->x, center->y, radius, 0.0, 6.28318530717959);
}

SlopeColor slope_color_parse(char c)
{
  switch (c)
    {
    case '0':
      return SLOPE_COLOR_NULL;
    case 'w':
      return SLOPE_WHITE;
    case 'r':
      return SLOPE_RED;
    case 'g':
      return SLOPE_GREEN;
    case 'b':
      return SLOPE_BLUE;
    case 'm':
      return SLOPE_MAROON;
    case 'y':
      return SLOPE_YELLOW;
    case 'l':
      return SLOPE_LIGHTSKYBLUE;
    case 't':
      return SLOPE_TEAL;
    default:
      return SLOPE_BLACK;
    }
  return SLOPE_BLACK;
}

/* slope/drawing.c */
