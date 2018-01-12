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

#include <slope/scale.h>
#include <slope/xyseries.h>

typedef struct _SlopeXySeriesPrivate
{
  double        x_min, x_max;
  double        y_min, y_max;
  const double *x_vec;
  const double *y_vec;
  long          n_pts;
  SlopeColor    line_color;
  SlopeColor    symbol_stroke_color;
  SlopeColor    symbol_fill_color;
  double        line_width;
  double        symbol_stroke_width;
  double        symbol_small_radius;
  double        symbol_big_radius;
  gboolean      antialias;
  int           mode;
} SlopeXySeriesPrivate;

static void _xyseries_draw(SlopeItem *self, cairo_t *cr);
static void _xyseries_draw_thumb(SlopeItem *       self,
                                 cairo_t *         cr,
                                 const SlopePoint *pos);
static void _xyseries_finalize(GObject *self);
static void _xyseries_get_figure_rect(SlopeItem *self, SlopeRect *rect);
static void _xyseries_get_data_rect(SlopeItem *self, SlopeRect *rect);
static void _xyseries_draw_line(SlopeXySeries *self, cairo_t *cr);
static void _xyseries_draw_circles(SlopeXySeries *self, cairo_t *cr);
static void _xyseries_draw_areaunder(SlopeXySeries *self, cairo_t *cr);

#define SLOPE_XYSERIES_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE(         \
      (obj), SLOPE_XYSERIES_TYPE, SlopeXySeriesPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(SlopeXySeries, slope_xyseries, SLOPE_ITEM_TYPE)

static void slope_xyseries_class_init(SlopeXySeriesClass *klass)
{
  GObjectClass *  object_klass = G_OBJECT_CLASS(klass);
  SlopeItemClass *item_klass   = SLOPE_ITEM_CLASS(klass);
  object_klass->finalize       = _xyseries_finalize;
  item_klass->draw             = _xyseries_draw;
  item_klass->draw_thumb       = _xyseries_draw_thumb;
  item_klass->get_data_rect    = _xyseries_get_data_rect;
  item_klass->get_figure_rect  = _xyseries_get_figure_rect;
}

static void slope_xyseries_init(SlopeXySeries *self)
{
  SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
  priv->n_pts                = 0L;
  priv->mode                 = SLOPE_SERIES_CIRCLES;
  priv->line_color           = SLOPE_BLUE;
  priv->symbol_stroke_color  = SLOPE_BLUE;
  priv->symbol_fill_color    = SLOPE_RED;
  priv->line_width           = 1.5;
  priv->symbol_stroke_width  = 1.0;
  priv->symbol_small_radius  = 3.0;
  priv->symbol_big_radius    = 4.0;
  priv->antialias            = TRUE;
}

void _xyseries_finalize(GObject *self)
{
  /* nothing to do */
  G_OBJECT_CLASS(slope_xyseries_parent_class)->finalize(self);
}

SlopeItem *slope_xyseries_new(void)
{
  SlopeItem *self = SLOPE_ITEM(g_object_new(SLOPE_XYSERIES_TYPE, NULL));
  return self;
}

SlopeItem *slope_xyseries_new_filled(const char *  name,
                                     const double *x_vec,
                                     const double *y_vec,
                                     long          n_pts,
                                     const char *  style)
{
  SlopeItem *self = SLOPE_ITEM(g_object_new(SLOPE_XYSERIES_TYPE, NULL));
  slope_item_set_name(self, name);
  slope_xyseries_update_data(SLOPE_XYSERIES(self), x_vec, y_vec, n_pts);
  slope_xyseries_set_style(SLOPE_XYSERIES(self), style);
  return self;
}

void slope_xyseries_set_data(SlopeXySeries *self,
                             const double * x_vec,
                             const double * y_vec,
                             long           n_pts)
{
  SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
  if (x_vec == NULL || y_vec == NULL || n_pts < 1L)
    {
      priv->n_pts = 0;
      return;
    }
  priv->x_vec = x_vec;
  priv->y_vec = y_vec;
  priv->n_pts = n_pts;
}

void slope_xyseries_update_data(SlopeXySeries *self,
                                const double * x_vec,
                                const double * y_vec,
                                long           n_pts)
{
  slope_xyseries_set_data(self, x_vec, y_vec, n_pts);
  slope_xyseries_update(self);
}

static void _xyseries_draw(SlopeItem *self, cairo_t *cr)
{
  SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
  if (priv->n_pts == 0L)
    {
      return;
    }
  slope_cairo_set_antialias(cr, priv->antialias);
  if (priv->mode == SLOPE_SERIES_LINE)
    {
      _xyseries_draw_line(SLOPE_XYSERIES(self), cr);
    }
  else if (priv->mode == SLOPE_SERIES_CIRCLES ||
           priv->mode == SLOPE_SERIES_BIGCIRCLES)
    {
      _xyseries_draw_circles(SLOPE_XYSERIES(self), cr);
    }
  else if (priv->mode == (SLOPE_SERIES_LINE | SLOPE_SERIES_CIRCLES) ||
           priv->mode == (SLOPE_SERIES_LINE | SLOPE_SERIES_BIGCIRCLES))
    {
      /* TODO: write a dedicate function to improve performance */
      _xyseries_draw_line(SLOPE_XYSERIES(self), cr);
      _xyseries_draw_circles(SLOPE_XYSERIES(self), cr);
    }
  else if (priv->mode == SLOPE_SERIES_AREAUNDER)
    {
      _xyseries_draw_areaunder(SLOPE_XYSERIES(self), cr);
    }
}

static void _xyseries_draw_thumb(SlopeItem *       self,
                                 cairo_t *         cr,
                                 const SlopePoint *pos)
{
  SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
  slope_cairo_set_antialias(cr, priv->antialias);
  /* TODO: can be improved very much */
  if (priv->mode == SLOPE_SERIES_LINE)
    {
      slope_cairo_set_color(cr, priv->line_color);
      cairo_set_line_width(cr, priv->line_width);
      cairo_move_to(cr, pos->x - 10.0, pos->y);
      cairo_line_to(cr, pos->x + 10.0, pos->y);
      cairo_stroke(cr);
    }
  else if (priv->mode == SLOPE_SERIES_CIRCLES ||
           priv->mode == SLOPE_SERIES_BIGCIRCLES)
    {
      cairo_set_line_width(cr, 1.1);
      slope_cairo_circle(cr, pos, 4.5);
      slope_cairo_draw(cr, priv->symbol_stroke_color, priv->symbol_fill_color);
    }
  else if (priv->mode == (SLOPE_SERIES_LINE | SLOPE_SERIES_CIRCLES) ||
           priv->mode == (SLOPE_SERIES_LINE | SLOPE_SERIES_BIGCIRCLES))
    {
      cairo_set_line_width(cr, 1.1);
      slope_cairo_circle(cr, pos, 4.5);
      slope_cairo_draw(cr, priv->symbol_stroke_color, priv->symbol_fill_color);
      slope_cairo_set_color(cr, priv->line_color);
      cairo_set_line_width(cr, priv->line_width);
      cairo_move_to(cr, pos->x - 10.0, pos->y);
      cairo_line_to(cr, pos->x + 10.0, pos->y);
      cairo_stroke(cr);
    }
  else if (priv->mode == SLOPE_SERIES_AREAUNDER)
    {
      slope_cairo_set_color(cr, priv->symbol_fill_color);
      cairo_rectangle(cr, pos->x - 10.0, pos->y - 6.0, 20.0, 12.0);
      cairo_fill(cr);
    }
}

static void _xyseries_draw_line(SlopeXySeries *self, cairo_t *cr)
{
  SlopeXySeriesPrivate *priv  = SLOPE_XYSERIES_GET_PRIVATE(self);
  SlopeScale *          scale = slope_item_get_scale(SLOPE_ITEM(self));
  SlopePoint            p1, p2, p;
  double                dx, dy, d2;
  long                  k;
  p.x = priv->x_vec[0];
  p.y = priv->y_vec[0];
  slope_scale_map(scale, &p1, &p);
  cairo_new_path(cr);
  cairo_move_to(cr, p1.x, p1.y);
  for (k = 1L; k < priv->n_pts; ++k)
    {
      p.x = priv->x_vec[k];
      p.y = priv->y_vec[k];
      slope_scale_map(scale, &p2, &p);
      dx = p2.x - p1.x;
      dy = p2.y - p1.y;
      d2 = dx * dx + dy * dy;
      if (d2 >= 9.0)
        {
          cairo_line_to(cr, p2.x, p2.y);
          p1 = p2;
        }
    }
  cairo_set_line_width(cr, priv->line_width);
  slope_cairo_set_color(cr, priv->symbol_stroke_color);
  cairo_stroke(cr);
}

static void _xyseries_draw_areaunder(SlopeXySeries *self, cairo_t *cr)
{
  SlopeXySeriesPrivate *priv  = SLOPE_XYSERIES_GET_PRIVATE(self);
  SlopeScale *          scale = slope_item_get_scale(SLOPE_ITEM(self));
  cairo_path_t *        data_path;
  SlopePoint            p1, p2, p0, p;
  double                dx, dy, d2;
  long                  k;
  p.x = priv->x_vec[0];
  p.y = priv->y_vec[0];
  slope_scale_map(scale, &p1, &p);
  cairo_new_path(cr);
  cairo_move_to(cr, p1.x, p1.y);
  /* keep track of the first point x and where the
   * x axis (y=0) is */
  p.y = 0.0;
  slope_scale_map(scale, &p0, &p);
  for (k = 1L; k < priv->n_pts; ++k)
    {
      p.x = priv->x_vec[k];
      p.y = priv->y_vec[k];
      slope_scale_map(scale, &p2, &p);
      dx = p2.x - p1.x;
      dy = p2.y - p1.y;
      d2 = dx * dx + dy * dy;
      if (d2 >= 9.0)
        {
          cairo_line_to(cr, p2.x, p2.y);
          p1 = p2;
        }
    }
  data_path = cairo_copy_path(cr);
  cairo_set_line_width(cr, priv->line_width);
  /* complete the closed path to fill */
  cairo_line_to(cr, p2.x, p0.y);
  cairo_line_to(cr, p0.x, p0.y);
  cairo_close_path(cr);
  slope_cairo_set_color(cr, priv->symbol_fill_color);
  cairo_fill(cr);
  cairo_append_path(cr, data_path);
  slope_cairo_set_color(cr, priv->symbol_stroke_color);
  cairo_set_line_width(cr, priv->symbol_stroke_width);
  cairo_stroke(cr);
  cairo_path_destroy(data_path);
}

static void _xyseries_draw_circles(SlopeXySeries *self, cairo_t *cr)
{
  SlopeXySeriesPrivate *priv  = SLOPE_XYSERIES_GET_PRIVATE(self);
  SlopeScale *          scale = slope_item_get_scale(SLOPE_ITEM(self));
  SlopePoint            dat_p, fig_p;
  cairo_set_line_width(cr, priv->line_width);
  long k;
  for (k = 0L; k < priv->n_pts; ++k)
    {
      fig_p.x = priv->x_vec[k];
      fig_p.y = priv->y_vec[k];
      slope_scale_map(scale, &dat_p, &fig_p);
      slope_cairo_circle(cr,
                         &dat_p,
                         (priv->mode & SLOPE_SERIES_BIGSYMBOL)
                             ? priv->symbol_big_radius
                             : priv->symbol_small_radius);
      slope_cairo_draw(cr, priv->symbol_stroke_color, priv->symbol_fill_color);
    }
}

static void _xyseries_get_figure_rect(SlopeItem *self, SlopeRect *rect)
{
  /* before we have a good "my rect procedure" use the scale's */
  slope_scale_get_figure_rect(slope_item_get_scale(self), rect);
}

static void _xyseries_get_data_rect(SlopeItem *self, SlopeRect *rect)
{
  SlopeXySeriesPrivate *priv = SLOPE_XYSERIES_GET_PRIVATE(self);
  rect->x                    = priv->x_min;
  rect->y                    = priv->y_min;
  rect->width                = priv->x_max - priv->x_min;
  rect->height               = priv->y_max - priv->y_min;
}

void slope_xyseries_update(SlopeXySeries *self)
{
  SlopeXySeriesPrivate *priv  = SLOPE_XYSERIES_GET_PRIVATE(self);
  SlopeScale *          scale = slope_item_get_scale(SLOPE_ITEM(self));
  const double *        x     = priv->x_vec;
  const double *        y     = priv->y_vec;
  priv->x_min = priv->x_max = x[0];
  priv->y_min = priv->y_max = x[0];
  long k;
  for (k = 1L; k < priv->n_pts; ++k)
    {
      if (x[k] < priv->x_min) priv->x_min = x[k];
      if (x[k] > priv->x_max) priv->x_max = x[k];
      if (y[k] < priv->y_min) priv->y_min = y[k];
      if (y[k] > priv->y_max) priv->y_max = y[k];
    }
  if (scale != NULL)
    {
      slope_scale_rescale(scale);
    }
}

int _xyseries_parse_mode(const char *c)
{
  int mode = 0;
  switch (c[0])
    {
    case 'o':
      {
        mode = SLOPE_SERIES_CIRCLES;
      }
      break;
    case 'O':
      {
        mode = SLOPE_SERIES_BIGCIRCLES;
      }
      break;
    case 'a':
      {
        mode = SLOPE_SERIES_AREAUNDER;
      }
      break;
    case '-':
      {
        mode = SLOPE_SERIES_LINE;
        if (c[1] != '\0')
          {
            switch (c[1])
              {
              case 'o':
                {
                  mode |= SLOPE_SERIES_CIRCLES;
                }
                break;
              case 'O':
                {
                  mode |= SLOPE_SERIES_BIGCIRCLES;
                }
                break;
              case 'a':
                {
                  mode |= SLOPE_SERIES_AREAUNDER;
                }
                break;
              }
          }
        break;
      }
    }
  return mode;
}

void slope_xyseries_set_style(SlopeXySeries *self, const char *style)
{
  SlopeXySeriesPrivate *priv       = SLOPE_XYSERIES_GET_PRIVATE(self);
  SlopeColor            fill_color = SLOPE_RED, stroke_color = SLOPE_BLUE;
  double                line_width          = 1.5;
  double                symbol_stroke_width = 1.0;
  int                   mode = SLOPE_SERIES_LINE, k = 0;
  /* parse the stroke and fill colors */
  if (style != NULL && style[k] != '\0')
    {
      stroke_color = slope_color_parse(style[k++]);
      fill_color   = stroke_color;
    }
  /* parse the mode (symbol) */
  if (style != NULL && style[k] != '\0')
    {
      mode = _xyseries_parse_mode(style + k++);
    }
  /* parse an optional fill color (if you like it
   * diferent from the stroke one) */
  if (style != NULL && style[k] != '\0')
    {
      fill_color = slope_color_parse(style[k++]);
    }
  if (fill_color == stroke_color)
    {
      /* for performance, if the fill and stroke colors
         are the same it is better to only fill the forms,
         or in the case of the line, only stroke it. */
      if (mode == SLOPE_SERIES_LINE)
        {
          fill_color = SLOPE_COLOR_NULL;
        }
      else
        {
          stroke_color = SLOPE_COLOR_NULL;
        }
    }
  if (mode & SLOPE_SERIES_AREAUNDER)
    {
      /* for "area under" plots it is cool to add transparency
         and a thicker line */
      SLOPE_SET_ALPHA(fill_color, 128);
      line_width = 1.5;
    }
  priv->mode                = mode;
  priv->line_color          = stroke_color;
  priv->symbol_fill_color   = fill_color;
  priv->symbol_stroke_color = stroke_color;
  priv->line_width          = line_width;
  priv->symbol_stroke_width = symbol_stroke_width;
}

/* slope/xyseries.c */
