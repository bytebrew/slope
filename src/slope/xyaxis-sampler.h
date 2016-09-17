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

#ifndef SLOPE_XYAXISSAMPLER_H
#define SLOPE_XYAXISSAMPLER_H

#include <slope/drawing.h>

#define SLOPE_XYAXIS_SAMPLE(ptr) ((SlopeXyAxisSample*) (ptr))

SLOPE_BEGIN_DECLS

typedef struct
_SlopeXyAxisSample
{
    double coord;
    char *label;
}
SlopeXyAxisSample;


typedef struct _SlopeXyAxisSampler  SlopeXyAxisSampler;


SlopeXyAxisSampler* slope_xyaxis_sampler_new (void);

void slope_xyaxis_sampler_destroy (SlopeXyAxisSampler *self);

void slope_xyaxis_sampler_clear (SlopeXyAxisSampler *self);

void slope_xyaxis_sampler_add_sample (SlopeXyAxisSampler *self, double coord, char *label);

void slope_xyaxis_sampler_set_samples (SlopeXyAxisSampler *self,
                                       SlopeXyAxisSample *sample_array, int n_samples);

GList* slope_xyaxis_sampler_get_sample_list (SlopeXyAxisSampler *self);

SLOPE_END_DECLS

#endif /* SLOPE_XYAXISSAMPLER_H */
