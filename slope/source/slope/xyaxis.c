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
#include <slope/xyaxis.h>

typedef struct _SlopeXyAxisPrivate
{
  guint32       orientation;
  guint32       component;
  double        min;
  double        max;
  double        anchor;
  SlopeColor    line_color;
  SlopeColor    grid_color;
  SlopeColor    text_color;
  SlopeColor    title_color;
  SlopeColor    select_rect_color;
  gboolean      line_antialias;
  double        line_width;
  double        grid_line_width;
  char *        title;
  gboolean      selected;
  SlopeSampler *sampler;
} SlopeXyAxisPrivate;

#define SLOPE_XYAXIS_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE((obj), SLOPE_XYAXIS_TYPE, SlopeXyAxisPrivate))

G_DEFINE_TYPE_WITH_PRIVATE(SlopeXyAxis, slope_xyaxis, SLOPE_ITEM_TYPE)

static void _xyaxis_finalize(GObject *self);
static void _xyaxis_get_figure_rect(SlopeItem *self, SlopeRect *rect);
static void _xyaxis_get_data_rect(SlopeItem *self, SlopeRect *rect);
static void _xyaxis_draw(SlopeItem *self, cairo_t *cr);
static void _xyaxis_draw_horizontal(SlopeXyAxis *self, cairo_t *cr);
static void _xyaxis_draw_vertical(SlopeXyAxis *self, cairo_t *cr);

static void slope_xyaxis_class_init(SlopeXyAxisClass *klass)
{
  GObjectClass *  object_klass = G_OBJECT_CLASS(klass);
  SlopeItemClass *item_klass   = SLOPE_ITEM_CLASS(klass);
  object_klass->finalize       = _xyaxis_finalize;
  item_klass->draw             = _xyaxis_draw;
  item_klass->get_data_rect    = _xyaxis_get_data_rect;
  item_klass->get_figure_rect  = _xyaxis_get_figure_rect;
}

static void slope_xyaxis_init(SlopeXyAxis *self)
{
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);
  priv->orientation        = SLOPE_HORIZONTAL;
  priv->line_color         = SLOPE_BLACK; /* SLOPE_GRAY(120); */
  priv->grid_color         = SLOPE_GRAY(120);
  priv->line_antialias     = FALSE;
  SLOPE_SET_ALPHA(priv->grid_color, 64);
  priv->text_color        = SLOPE_BLACK;
  priv->select_rect_color = SLOPE_BLUE;
  SLOPE_SET_ALPHA(priv->select_rect_color, 100);
  priv->title_color     = SLOPE_BLACK;
  priv->line_width      = 1.0;
  priv->grid_line_width = 1.0;
  priv->title           = NULL;
  priv->selected        = FALSE;
  priv->component =
      SLOPE_XYAXIS_LINE | SLOPE_XYAXIS_TICKS_DOWN | SLOPE_XYAXIS_TITLE;
  priv->sampler = slope_sampler_new();
}

static void _xyaxis_finalize(GObject *self)
{
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);
  slope_sampler_destroy(priv->sampler);
  G_OBJECT_CLASS(slope_xyaxis_parent_class)->finalize(self);
}

SlopeItem *slope_xyaxis_new(int orientation, const char *title)
{
  SlopeXyAxis *self = SLOPE_XYAXIS(g_object_new(SLOPE_XYAXIS_TYPE, NULL));
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);
  priv->orientation        = orientation;
  slope_xyaxis_set_title(self, title);
  return SLOPE_ITEM(self);
}

static void _xyaxis_draw(SlopeItem *self, cairo_t *cr)
{
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);
  cairo_set_line_width(cr, priv->line_width);
  slope_cairo_set_antialias(cr, priv->line_antialias);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  if (priv->orientation == SLOPE_HORIZONTAL)
    {
      _xyaxis_draw_horizontal(SLOPE_XYAXIS(self), cr);
    }
  else if (priv->orientation == SLOPE_VERTICAL)
    {
      _xyaxis_draw_vertical(SLOPE_XYAXIS(self), cr);
    }
  if (priv->selected == TRUE)
    {
      SlopeRect rect;
      slope_item_get_figure_rect(self, &rect);
      slope_cairo_set_color(cr, priv->select_rect_color);
      slope_cairo_rect(cr, &rect);
      cairo_fill(cr);
    }
}

static void _xyaxis_draw_horizontal(SlopeXyAxis *self, cairo_t *cr)
{
  SlopeXyAxisPrivate * priv  = SLOPE_XYAXIS_GET_PRIVATE(self);
  SlopeScale *         scale = slope_item_get_scale(SLOPE_ITEM(self));
  cairo_text_extents_t txt_ext;
  SlopeRect            scale_fig_rect;
  SlopePoint           p, p1, p2, pt1, pt2;
  GList *              sample_list, *iter;
  double               txt_height;
  guint32              sampler_mode;
  slope_scale_get_figure_rect(scale, &scale_fig_rect);
  cairo_text_extents(cr, "dummy", &txt_ext);
  txt_height = txt_ext.height;

  p.x = priv->min;
  p.y = priv->anchor;
  slope_scale_map(scale, &p1, &p);

  p.x = priv->max;
  p.y = priv->anchor;
  slope_scale_map(scale, &p2, &p);

  if (priv->component & SLOPE_XYAXIS_LINE)
    {
      cairo_new_path(cr);
      slope_cairo_set_color(cr, priv->line_color);
      slope_cairo_line_cosmetic(cr, &p1, &p2, priv->line_width);
      cairo_stroke(cr);

      if (priv->component == SLOPE_XYAXIS_LINE)
        {
          return;
        }
    }

  sampler_mode = slope_sampler_get_mode(priv->sampler);
  if (sampler_mode == SLOPE_SAMPLER_AUTO_DECIMAL)
    {
      slope_sampler_auto_sample_decimal(
          priv->sampler, priv->min, priv->max, (p2.x - p1.x) / 80.0);
    }

  sample_list = slope_sampler_get_sample_list(priv->sampler);
  pt1.y       = scale_fig_rect.y;
  pt2.y       = scale_fig_rect.y + scale_fig_rect.height;
  iter        = sample_list;

  while (iter != NULL)
    {
      SlopeSample *sample;
      SlopePoint   sample_p1, sample_p2;

      sample = SLOPE_XYAXIS_SAMPLE(iter->data);
      iter   = iter->next;
      if (sample->coord < priv->min || sample->coord > priv->max)
        {
          continue;
        }

      p.x = sample->coord;
      p.y = priv->anchor;
      slope_scale_map(scale, &sample_p1, &p);
      sample_p2 = sample_p1;
      sample_p2.y += (priv->component & SLOPE_XYAXIS_TICKS_DOWN) ? -4.0 : 4.0;

      if (priv->component & SLOPE_XYAXIS_GRID)
        {
          cairo_save(cr);
          slope_cairo_set_color(cr, priv->grid_color);
          pt1.x = pt2.x = sample_p1.x;
          slope_cairo_line_cosmetic(cr, &pt1, &pt2, priv->grid_line_width);
          cairo_stroke(cr);
          cairo_restore(cr);
        }
      else if (priv->component & SLOPE_XYAXIS_TICKS_DOWN ||
               priv->component & SLOPE_XYAXIS_TICKS_UP)
        {
          slope_cairo_set_color(cr, priv->line_color);
          slope_cairo_line_cosmetic(
              cr, &sample_p1, &sample_p2, priv->line_width);
          cairo_stroke(cr);
        }

      if (sample->label != NULL && (priv->component & SLOPE_XYAXIS_TICKS_DOWN ||
                                    priv->component & SLOPE_XYAXIS_TICKS_UP))
        {
          cairo_text_extents(cr, sample->label, &txt_ext);
          slope_cairo_set_color(cr, priv->text_color);
          slope_cairo_text(
              cr,
              sample_p1.x - txt_ext.width * 0.5,
              sample_p1.y + ((priv->component & SLOPE_XYAXIS_TICKS_DOWN)
                                 ? txt_height * 1.0
                                 : -txt_height * 0.3),
              sample->label);
        }
    }

  if (priv->title != NULL && (priv->component & SLOPE_XYAXIS_TITLE))
    {
      cairo_text_extents(cr, priv->title, &txt_ext);
      slope_cairo_set_color(cr, priv->title_color);
      if (priv->component & SLOPE_XYAXIS_TICKS_DOWN)
        {
          slope_cairo_text(cr,
                           (p1.x + p2.x - txt_ext.width) / 2.0,
                           p1.y + txt_height * 2.5,
                           priv->title);
        }
      else if (priv->component & SLOPE_XYAXIS_TICKS_UP)
        {
          slope_cairo_text(cr,
                           (p1.x + p2.x - txt_ext.width) / 2.0,
                           p1.y - txt_height * 1.8,
                           priv->title);
        }
      else
        {
          slope_cairo_text(cr,
                           (p1.x + p2.x - txt_ext.width) / 2.0,
                           p1.y + txt_height * 1.3,
                           priv->title);
        }
    }
}

static void _xyaxis_draw_vertical(SlopeXyAxis *self, cairo_t *cr)
{
  SlopeXyAxisPrivate * priv  = SLOPE_XYAXIS_GET_PRIVATE(self);
  SlopeScale *         scale = slope_item_get_scale(SLOPE_ITEM(self));
  cairo_text_extents_t txt_ext;
  SlopeRect            scale_fig_rect;
  SlopePoint           p, p1, p2, pt1, pt2;
  GList *              sample_list, *iter;
  double               txt_height, max_txt_width = 0.0;
  guint32              sampler_mode;

  slope_scale_get_figure_rect(scale, &scale_fig_rect);
  cairo_text_extents(cr, "dummy", &txt_ext);
  txt_height = txt_ext.height;

  p.x = priv->anchor;
  p.y = priv->min;
  slope_scale_map(scale, &p1, &p);

  p.x = priv->anchor;
  p.y = priv->max;
  slope_scale_map(scale, &p2, &p);

  if (priv->component & SLOPE_XYAXIS_LINE)
    {
      cairo_new_path(cr);
      slope_cairo_set_color(cr, priv->line_color);
      slope_cairo_line_cosmetic(cr, &p1, &p2, priv->line_width);
      cairo_stroke(cr);

      if (priv->component == SLOPE_XYAXIS_LINE)
        {
          return;
        }
    }

  sampler_mode = slope_sampler_get_mode(priv->sampler);
  if (sampler_mode == SLOPE_SAMPLER_AUTO_DECIMAL)
    {
      slope_sampler_auto_sample_decimal(
          priv->sampler, priv->min, priv->max, (p1.y - p2.y) / 80.0);
    }

  sample_list = slope_sampler_get_sample_list(priv->sampler);
  iter        = sample_list;
  pt1.x       = scale_fig_rect.x;
  pt2.x       = scale_fig_rect.x + scale_fig_rect.width;

  while (iter != NULL)
    {
      SlopeSample *sample;
      SlopePoint   sample_p1, sample_p2;

      sample = SLOPE_XYAXIS_SAMPLE(iter->data);
      iter   = iter->next;
      if (sample->coord < priv->min || sample->coord > priv->max)
        {
          continue;
        }

      p.x = priv->anchor;
      p.y = sample->coord;
      slope_scale_map(scale, &sample_p1, &p);
      sample_p2 = sample_p1;
      sample_p2.x += (priv->component & SLOPE_XYAXIS_TICKS_DOWN) ? +4.0 : -4.0;

      if (priv->component & SLOPE_XYAXIS_GRID)
        {
          cairo_save(cr);
          slope_cairo_set_color(cr, priv->grid_color);
          pt1.y = pt2.y = sample_p1.y;
          slope_cairo_line_cosmetic(cr, &pt1, &pt2, priv->grid_line_width);
          cairo_stroke(cr);
          cairo_restore(cr);
        }
      else if (priv->component & SLOPE_XYAXIS_TICKS_DOWN ||
               priv->component & SLOPE_XYAXIS_TICKS_UP)
        {
          slope_cairo_set_color(cr, priv->line_color);
          slope_cairo_line_cosmetic(
              cr, &sample_p1, &sample_p2, priv->line_width);
          cairo_stroke(cr);
        }

      if (sample->label != NULL && (priv->component & SLOPE_XYAXIS_TICKS_DOWN ||
                                    priv->component & SLOPE_XYAXIS_TICKS_UP))
        {
          cairo_text_extents(cr, sample->label, &txt_ext);
          if (txt_ext.width > max_txt_width) max_txt_width = txt_ext.width;
          slope_cairo_set_color(cr, priv->text_color);
          slope_cairo_text(
              cr,
              sample_p1.x + ((priv->component & SLOPE_XYAXIS_TICKS_DOWN)
                                 ? -txt_ext.width - txt_height * 0.3
                                 : +txt_height * 0.3),
              sample_p1.y + txt_height * 0.34,
              sample->label);
        }
    }

  if (priv->title != NULL && (priv->component & SLOPE_XYAXIS_TITLE))
    {
      cairo_save(cr);
      cairo_rotate(cr, -1.5707963267949);
      cairo_text_extents(cr, priv->title, &txt_ext);
      slope_cairo_set_color(cr, priv->title_color);
      if (priv->component & SLOPE_XYAXIS_TICKS_DOWN)
        {
          slope_cairo_text(cr,
                           -((p1.y + p2.y) + txt_ext.width) / 2.0,
                           p1.x - max_txt_width - 1.0 * txt_height,
                           priv->title);
        }
      else if (priv->component & SLOPE_XYAXIS_TICKS_UP)
        {
          slope_cairo_text(cr,
                           -((p1.y + p2.y) + txt_ext.width) / 2.0,
                           p1.x + max_txt_width + 1.6 * txt_height,
                           priv->title);
        }
      else
        {
          slope_cairo_text(cr,
                           -((p1.y + p2.y) + txt_ext.width) / 2.0,
                           p1.x + 0.3 * txt_height,
                           priv->title);
        }
      cairo_restore(cr);
    }
}

void slope_xyaxis_set_position(SlopeXyAxis *self,
                               double       min,
                               double       max,
                               double       anchor)
{
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);

  priv->min    = min;
  priv->max    = max;
  priv->anchor = anchor;
}

void slope_xyaxis_set_components(SlopeXyAxis *self, guint32 components)
{
  SLOPE_XYAXIS_GET_PRIVATE(self)->component = components;
}

static void _xyaxis_get_figure_rect(SlopeItem *self, SlopeRect *rect)
{
  SlopeXyAxisPrivate *priv  = SLOPE_XYAXIS_GET_PRIVATE(self);
  SlopeScale *        scale = slope_item_get_scale(self);
  SlopePoint          p1, p2, p;

  if (priv->orientation == SLOPE_HORIZONTAL)
    {
      p.x = priv->min;
      p.y = priv->anchor;
      slope_scale_map(scale, &p1, &p);

      p.x = priv->max;
      p.y = priv->anchor;
      slope_scale_map(scale, &p2, &p);

      rect->x      = p1.x;
      rect->y      = p1.y - 4.0;
      rect->width  = p2.x - p1.x;
      rect->height = 8.0;
    }
  else
    {
      p.x = priv->anchor;
      p.y = priv->min;
      slope_scale_map(scale, &p1, &p);

      p.x = priv->anchor;
      p.y = priv->max;
      slope_scale_map(scale, &p2, &p);

      rect->x      = p1.x - 4.0;
      rect->y      = p1.y;
      rect->width  = 8.0;
      rect->height = p2.y - p1.y;
    }
}

static void _xyaxis_get_data_rect(SlopeItem *self, SlopeRect *rect)
{
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);

  if (priv->orientation == SLOPE_HORIZONTAL)
    {
      rect->x      = priv->min;
      rect->y      = priv->anchor;
      rect->width  = priv->max - priv->min;
      rect->height = 0.0;
    }
  else
    {
      rect->x      = priv->anchor;
      rect->y      = priv->min;
      rect->width  = 0.0;
      rect->height = priv->max - priv->min;
    }
}

void slope_xyaxis_set_title(SlopeXyAxis *self, const char *title)
{
  SlopeXyAxisPrivate *priv = SLOPE_XYAXIS_GET_PRIVATE(self);
  if (priv->title != NULL)
    {
      g_free(priv->title);
    }
  if (title != NULL)
    {
      priv->title = g_strdup(title);
    }
  else
    {
      priv->title = NULL;
    }
}

SlopeSampler *slope_xyaxis_get_sampler(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->sampler;
}

const char *slope_xyaxis_get_title(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->title;
}

gboolean slope_xyaxis_get_selected(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->selected;
}

void slope_xyaxis_set_selected(SlopeXyAxis *self, gboolean selected)
{
  SLOPE_XYAXIS_GET_PRIVATE(self)->selected = selected;
}

void slope_xyaxis_set_line_color(SlopeXyAxis *self, SlopeColor color)
{
  SLOPE_XYAXIS_GET_PRIVATE(self)->line_color = color;
}

SlopeColor slope_xyaxis_get_line_color(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->line_color;
}

void slope_xyaxis_set_grid_color(SlopeXyAxis *self, SlopeColor color)
{
  SLOPE_XYAXIS_GET_PRIVATE(self)->grid_color = color;
}

SlopeColor slope_xyaxis_get_grid_color(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->grid_color;
}

void slope_xyaxis_set_title_color(SlopeXyAxis *self, SlopeColor color)
{
  SLOPE_XYAXIS_GET_PRIVATE(self)->title_color = color;
}

SlopeColor slope_xyaxis_get_title_color(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->title_color;
}

void slope_xyaxis_set_selection_color(SlopeXyAxis *self, SlopeColor color)
{
  SLOPE_XYAXIS_GET_PRIVATE(self)->select_rect_color = color;
}

SlopeColor slope_xyaxis_get_selection_color(SlopeXyAxis *self)
{
  return SLOPE_XYAXIS_GET_PRIVATE(self)->select_rect_color;
}

/* slope/xyaxis.c */
