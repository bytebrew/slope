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

#ifndef __SLOPE_SAMPLER__
#define __SLOPE_SAMPLER__

#include <slope/list.h>

#define SLOPE_SAMPLE(instance) ((slope_sample_t*) (instance))
#define SLOPE_SAMPLER(instance) ((slope_sampler_t*) (instance))

SLOPE_BEGIN_DECLS

typedef struct _slope_sampler slope_sampler_t;
typedef struct _slope_sample slope_sample_t;


struct _slope_sample
{
   double value;
   char *label;
   slope_bool_t is_major;
};


typedef enum _slope_sampler_mode
{
    SLOPE_SAMPLER_AUTO_DECIMAL,
    SLOPE_SAMPLER_CUSTOM_SAMPLES
}
slope_sampler_mode_t;


slope_sampler_t* slope_sampler_new ();

void slope_sampler_destroy (slope_sampler_t *self);

slope_list_t* slope_sampler_get_sample_list (slope_sampler_t *self);

void slope_sampler_clear (slope_sampler_t *self);

void slope_sampler_add_sample (slope_sampler_t *self, double value, const char *label, slope_bool_t is_major);

void slope_sampler_set_samples (slope_sampler_t *self, const slope_sample_t *samples, int sample_count);

void slope_sampler_set_mode (slope_sampler_t *self, slope_sampler_mode_t mode);

slope_sampler_mode_t slope_sampler_get_mode (const slope_sampler_t *self);

void slope_sampler_auto_sample_decimal (slope_sampler_t *self, double min, double max, double hint);

SLOPE_END_DECLS

#endif /*__SLOPE_SAMPLER__*/
