/*
 * Copyright (C) 2015  Elvis Teixeira
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

#include <slope/sampler.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


struct _slope_sampler
{
   slope_list_t *sample_list;
   slope_sampler_mode_t mode;
   double min, max;
   double hint;
};


slope_sampler_t* slope_sampler_new ()
{
   slope_sampler_t *self = SLOPE_ALLOC(slope_sampler_t);
   self->sample_list = slope_list_new();
   self->min = 0.0;
   self->max = 0.0;
   self->hint = 0.0;
   self->mode = SLOPE_SAMPLER_AUTO_DECIMAL;
   return self;
}


void slope_sampler_destroy (slope_sampler_t *self)
{
   slope_sampler_clear(self);
   slope_list_destroy(self->sample_list);
   SLOPE_FREE(self);
}


slope_list_t* slope_sampler_get_sample_list (slope_sampler_t *self)
{
   return self->sample_list;
}


void slope_sampler_clear (slope_sampler_t *self)
{
   slope_iterator_t *iter;
   SLOPE_LIST_FOREACH (iter, self->sample_list) {
      slope_sample_t *sample = SLOPE_SAMPLE(slope_iterator_data(iter));
      SLOPE_FREE(sample->label);
      SLOPE_FREE(sample);
   }
   slope_list_clear(self->sample_list);
}


void slope_sampler_add_sample (slope_sampler_t *self, double value, const char *label, slope_bool_t is_major)
{
   slope_sample_t *sample = SLOPE_ALLOC(slope_sample_t);
   sample->value = value;
   sample->label = (label != NULL) ? strdup(label) : NULL;
   sample->is_major = is_major;
   slope_list_append(self->sample_list, sample);
}


void slope_sampler_set_samples (slope_sampler_t *self, const slope_sample_t *samples, int sample_count)
{
   int k;
   self->mode = SLOPE_SAMPLER_CUSTOM_SAMPLES;
   slope_sampler_clear(self);
   for (k=0; k<sample_count; k++)
      slope_sampler_add_sample(self, samples[k].value, samples[k].label, SLOPE_TRUE);
}


void slope_sampler_auto_sample_decimal (slope_sampler_t *self, double min, double max, double hint)
{
    double coord;
    double first_tick;
    double v_diff, pow_diff;
    double samp_spac;
    int k;
    
    if (self->min == min && self->max == max && self->hint == hint) {
        return;
    }
    self->min = min;
    self->max = max;
    self->hint = hint;
    
    v_diff = max - min;
    pow_diff = round(log10(v_diff));
    samp_spac = pow(10.0, pow_diff-1.0);
   
    if ((v_diff/samp_spac) > (hint+5.0))
       samp_spac *= 5.0;
    if ((v_diff/samp_spac) < (hint-5.0))
       samp_spac /= 5.0;
   
    if (min < 0.0) {
        first_tick = -floor(fabs(min)/samp_spac) * samp_spac;
    } else {
        first_tick = floor(fabs(min)/samp_spac + 1.0) * samp_spac;
    }

    slope_sampler_clear(self);
    samp_spac /= 4.0;
    coord = first_tick;
    k = 0;
    while (coord <= max) {
       char buf[16];
       slope_bool_t is_major = (k%4 == 0) ? SLOPE_TRUE : SLOPE_FALSE;
       if (is_major) {
            /* sometimes 0.0 is displayed -0.0, thats weird */
            if (coord == -0.0) coord = 0.0;
            sprintf(buf, "%2.2f", coord);
            slope_sampler_add_sample(self, coord, buf, SLOPE_TRUE);
       }
       else {
           slope_sampler_add_sample(self, coord, NULL, SLOPE_FALSE);
       }
       coord += samp_spac;
       k += 1;
    }
}


void slope_sampler_set_mode (slope_sampler_t *self, slope_sampler_mode_t mode)
{
    self->mode = mode;
}


slope_sampler_mode_t slope_sampler_get_mode (const slope_sampler_t *self)
{
    return self->mode;
}

/* slope/sampler.c */
