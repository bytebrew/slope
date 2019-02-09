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
    self->size = 0UL;
    self->min = 0.0;
    self->max = 1.0;
    self->hint = 40.0;
    self->auto_sample = 1;
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


void slope_sampler_auto_sample_decimal(SlopeSampler *self, double min, double max, double hint)
{
    double coord;
    double first_tick;
    double v_diff, pow_diff;
    double samp_spac;
    int k;

    if (slope_float_similar(self->min, min)
            && slope_float_similar(self->max, max)
            && slope_float_similar(self->hint, hint)) {
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

    if (min < 0.0) {
        first_tick = -floor(fabs(min) / samp_spac) * samp_spac;
    } else {
        first_tick = floor(fabs(min) / samp_spac + 1.0) * samp_spac;
    }

    slope_sampler_clear(self);
    coord = first_tick;
    k = 0;

    while (coord <= max) {
        static const char *const format[] = {"%2.1f", "%2.1e"};
        int format_idx = 0;
        SlopeSample sample;

        /* sometimes 0.0 is displayed -0.0, or even something different
        * than zerothats weird */
        if (slope_float_similar_zero(coord)
                || slope_float_similar_zero(fabs(coord) / v_diff)) {
            coord = 0.0;
        }

        /* if numbers are too extreme use power of ten notation */
        if (coord > 1e4 || coord < -1e4) format_idx = 1;

        sprintf(sample.label, format[format_idx], coord);
        sample.value = coord;
        slope_sampler_add_sample(self, &sample);

        coord += samp_spac;
        k += 1;
    }
}
