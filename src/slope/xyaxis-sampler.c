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

#include <slope/xyaxis-sampler.h>


typedef struct
_SlopeXyAxisSampler
{
    GList *sample_list;
}
SlopeXyAxisSampler;


void _xyaxis_sampler_delete_sample (gpointer data);



SlopeXyAxisSampler *slope_xyaxis_sampler_new (void)
{
    SlopeXyAxisSampler *self = g_malloc(sizeof(SlopeXyAxisSampler));

    self->sample_list = NULL;

    return self;
}


void slope_xyaxis_sampler_destroy (SlopeXyAxisSampler *self)
{
    slope_xyaxis_sampler_clear(self);
    g_free(self);
}


void slope_xyaxis_sampler_clear (SlopeXyAxisSampler *self)
{
    g_list_free_full(self->sample_list, _xyaxis_sampler_delete_sample);
    self->sample_list = NULL;
}


void slope_xyaxis_sampler_add_sample (SlopeXyAxisSampler *self, double coord, char *label)
{
    SlopeXyAxisSample *sample = g_malloc(sizeof(SlopeXyAxisSample));

    sample->coord = coord;
    sample->label = label != NULL ? g_strdup(label) : NULL;

    self->sample_list = g_list_append(self->sample_list, sample);
}


void slope_xyaxis_sampler_set_samples (SlopeXyAxisSampler *self,
                                       SlopeXyAxisSample *sample_array, int n_samples)
{
    int k;
    for (k=0; k<n_samples; ++k) {
        slope_xyaxis_sampler_add_sample(self, sample_array[k].coord, sample_array[k].label);
    }
}


GList* slope_xyaxis_sampler_get_sample_list (SlopeXyAxisSampler *self)
{
    return self->sample_list;
}


void _xyaxis_sampler_delete_sample (gpointer data)
{
    g_free(((SlopeXyAxisSample*) data)->label);
    g_free(data);
}

/* slope/xyaxis-sampler.c */
