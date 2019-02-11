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

#ifndef _slope_sampler_h_
#define _slope_sampler_h_

#include "slope/global.h"

#define SLOPE_SAMPLER_MAX_LABEL 64
#define SLOPE_SAMPLER_MAX_SAMPLES 64

G_BEGIN_DECLS

typedef struct _SlopeSample SlopeSample;
#define SLOPE_SAMPLE(Addr) ((SlopeSample *) (Addr))

typedef struct _SlopeSampler SlopeSampler;
#define SLOPE_SAMPLER(Addr) ((SlopeSampler *) (Addr))

struct _SlopeSample {
    double value;
    char label[SLOPE_SAMPLER_MAX_LABEL];
};

struct _SlopeSampler {
    double min;
    double hint;
    double max;
    gulong size;
    gulong auto_sample : 1;
    SlopeSample samples[SLOPE_SAMPLER_MAX_SAMPLES];
};


void slope_sampler_init (SlopeSampler *self);

void slope_sampler_add_sample (SlopeSampler *self, const SlopeSample *sample);

void slope_sampler_clear (SlopeSampler *self);

G_END_DECLS

#endif /* _slope_sampler_h_ */
