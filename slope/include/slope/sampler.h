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

#ifndef SLOPE_SAMPLER_H
#define SLOPE_SAMPLER_H

#include <slope/drawing.h>

#define SLOPE_XYAXIS_SAMPLE(ptr) ((SlopeSample *) (ptr))

SLOPE_BEGIN_DECLS

typedef enum _SlopeSamplerMode {
  SLOPE_SAMPLER_MANUAL,
  SLOPE_SAMPLER_AUTO_DECIMAL
} SlopeSamplerMode;

typedef struct _SlopeSample
{
  double coord;
  char * label;
} SlopeSample;

typedef struct _SlopeSampler SlopeSampler;

SlopeSampler *slope_sampler_new(void);

void slope_sampler_destroy(SlopeSampler *self);

void slope_sampler_clear(SlopeSampler *self);

void slope_sampler_add_sample(SlopeSampler *self, double coord, char *label);

void slope_sampler_set_samples(SlopeSampler *     self,
                               const SlopeSample *sample_array,
                               int                n_samples);

GList *slope_sampler_get_sample_list(SlopeSampler *self);

guint32 slope_sampler_get_mode(SlopeSampler *self);

void slope_sampler_auto_sample_decimal(SlopeSampler *self,
                                       double        min,
                                       double        max,
                                       double        hint);

extern const SlopeSample *const slope_sampler_pi_samples;

extern const SlopeSample *const slope_sampler_month_samples;

SLOPE_END_DECLS

#endif /* SLOPE_SAMPLER_H */
