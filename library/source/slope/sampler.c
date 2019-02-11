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

#include "slope/sampler.h"
#include <stdio.h>
#include <math.h>

void
slope_sampler_init (SlopeSampler *self)
{
    g_assert(self);
    self->size = 0UL;
}


void
slope_sampler_add_sample (SlopeSampler *self, const SlopeSample *sample)
{
    g_assert(self);
    g_assert(sample);
    g_assert(self->size < SLOPE_SAMPLER_MAX_LABEL);

    self->samples[self->size].value = sample->value;
    g_strlcpy(self->samples[self->size].label, sample->label, SLOPE_SAMPLER_MAX_LABEL);
    self->size += 1UL;
}


void slope_sampler_clear (SlopeSampler *self)
{
    g_assert(self);
    self->size = 0UL;
}
