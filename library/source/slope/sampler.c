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


void slope_sample_guess_decimal_spacing (double *min, double *max, double *cut_count)
{
    double diff, pow_diff, samp_spac;
    double hint, first_tick;

    g_assert(min && max && cut_count);
    g_assert(*max > *min);

    diff = *max - *min;
    pow_diff = round(log10(diff));
    samp_spac = pow(10.0, pow_diff - 1.0);

    /* HACK: Possibly adjusts the spacing to avoid cases in which only
     * one or two divisions */
    hint = *cut_count;
    if (hint > 1.0 && hint < 20.0) {
        if ((diff / samp_spac) > (hint + 5.0)) samp_spac *= 2.0;
        if ((diff / samp_spac) < (hint - 5.0)) samp_spac /= 2.0;
        if ((diff / samp_spac) > (hint + 5.0)) samp_spac *= 2.0;
        if ((diff / samp_spac) < (hint - 5.0)) samp_spac /= 2.0;
    }

    if (*min < 0.0) {
        first_tick = -floor(fabs(*min) / samp_spac) * samp_spac;
    } else {
        first_tick = floor(fabs(*min) / samp_spac + 1.0) * samp_spac;
    }

   *cut_count = diff / samp_spac;
   *min = first_tick;
   *max = *min + (*cut_count) * samp_spac;
}
