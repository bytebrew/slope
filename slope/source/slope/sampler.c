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
#include <slope/sampler.h>
#include <stdio.h>

typedef struct _SlopeSampler
{
  GList * sample_list;
  guint32 mode;
  double  min;
  double  max;
  double  hint;
} SlopeSampler;

void _sampler_delete_sample(gpointer data);

SlopeSampler *slope_sampler_new(void)
{
  SlopeSampler *self = g_malloc(sizeof(SlopeSampler));

  self->sample_list = NULL;
  self->min         = 0.0;
  self->max         = 0.0;
  self->mode        = SLOPE_SAMPLER_AUTO_DECIMAL;

  return self;
}

void slope_sampler_destroy(SlopeSampler *self)
{
  slope_sampler_clear(self);
  g_free(self);
}

void slope_sampler_clear(SlopeSampler *self)
{
  g_list_free_full(self->sample_list, _sampler_delete_sample);
  self->sample_list = NULL;
}

void slope_sampler_add_sample(SlopeSampler *self, double coord, char *label)
{
  SlopeSample *sample = g_malloc(sizeof(SlopeSample));

  sample->coord = coord;
  sample->label = label != NULL ? g_strdup(label) : NULL;

  self->sample_list = g_list_append(self->sample_list, sample);
}

void slope_sampler_set_samples(SlopeSampler *     self,
                               const SlopeSample *sample_array,
                               int                n_samples)
{
  int k;

  self->mode = SLOPE_SAMPLER_MANUAL;
  for (k = 0; k < n_samples; ++k)
    {
      slope_sampler_add_sample(
          self, sample_array[k].coord, sample_array[k].label);
    }
}

GList *slope_sampler_get_sample_list(SlopeSampler *self)
{
  return self->sample_list;
}

void _sampler_delete_sample(gpointer data)
{
  g_free(((SlopeSample *) data)->label);
  g_free(data);
}

guint32 slope_sampler_get_mode(SlopeSampler *self) { return self->mode; }
void slope_sampler_auto_sample_decimal(SlopeSampler *self,
                                       double        min,
                                       double        max,
                                       double        hint)
{
  double coord;
  double first_tick;
  double v_diff, pow_diff;
  double samp_spac;
  int    k;

  if (self->min == min && self->max == max && self->hint == hint)
    {
      return;
    }

  self->min  = min;
  self->max  = max;
  self->hint = hint;

  v_diff    = max - min;
  pow_diff  = round(log10(v_diff));
  samp_spac = pow(10.0, pow_diff - 1.0);

  if ((v_diff / samp_spac) > (hint + 5.0)) samp_spac *= 2.0;
  if ((v_diff / samp_spac) < (hint - 5.0)) samp_spac /= 2.0;
  if ((v_diff / samp_spac) > (hint + 5.0)) samp_spac *= 2.0;
  if ((v_diff / samp_spac) < (hint - 5.0)) samp_spac /= 2.0;

  if (min < 0.0)
    {
      first_tick = -floor(fabs(min) / samp_spac) * samp_spac;
    }
  else
    {
      first_tick = floor(fabs(min) / samp_spac + 1.0) * samp_spac;
    }

  slope_sampler_clear(self);
  coord = first_tick;
  k     = 0;

  while (coord <= max)
    {
      char                     buf[16];
      static const char *const format[]   = {"%2.1f", "%2.1e"};
      int                      format_idx = 0;

      /* sometimes 0.0 is displayed -0.0, or even something different
         than zerothats weird */
      if (coord == -0.0) coord                 = 0.0;
      if ((fabs(coord) / v_diff) < 1e-4) coord = 0.0;
      /* if numbers are too extreme use power of ten notation */
      if (coord > 1e4 || coord < -1e4) format_idx = 1;

      sprintf(buf, format[format_idx], coord);
      slope_sampler_add_sample(self, coord, buf);

      coord += samp_spac;
      k += 1;
    }
}

const SlopeSample slope_sampler_pi_samples_array[] = {
    {0.0 * G_PI, "0"},     {0.5 * G_PI, "π/2"},   {1.0 * G_PI, "π"},
    {1.5 * G_PI, "3π/2"},  {2.0 * G_PI, "2π"},    {2.5 * G_PI, "5π/2"},
    {3.0 * G_PI, "3π"},    {3.5 * G_PI, "7π/2"},  {4.0 * G_PI, "4π"},
    {4.5 * G_PI, "9π/2"},  {5.0 * G_PI, "5π"},    {5.5 * G_PI, "11π/2"},
    {6.0 * G_PI, "6π"},    {6.5 * G_PI, "13π/2"}, {7.0 * G_PI, "7π"},
    {7.5 * G_PI, "15π/2"}, {8.0 * G_PI, "8π"},    {8.5 * G_PI, "17π/2"},
    {9.0 * G_PI, "9π"},    {9.5 * G_PI, "19π/2"}, {10.0 * G_PI, "20π"}};

const SlopeSample *const slope_sampler_pi_samples =
    slope_sampler_pi_samples_array;

const SlopeSample slope_sampler_month_samples_array[] = {{1.0, "Jan"},
                                                         {2.0, "Feb"},
                                                         {3.0, "Mar"},
                                                         {4.0, "Apr"},
                                                         {5.0, "May"},
                                                         {6.0, "Jun"},
                                                         {7.0, "Jul"},
                                                         {8.0, "Aug"},
                                                         {9.0, "Sep"},
                                                         {10.0, "Oct"},
                                                         {11.0, "Nov"},
                                                         {12.0, "Dez"}};

const SlopeSample *const slope_sampler_month_samples =
    slope_sampler_month_samples_array;

/* slope/xyaxis-sampler.c */
