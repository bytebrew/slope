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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "slope/legend_p.h"
#include "slope/figure_p.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


slope_item_class_t* __slope_legend_get_class()
{
    static int first_call = SLOPE_TRUE;
    static slope_item_class_t klass;

    if (first_call) {
        klass.destroy_fn = NULL;
        klass.draw_fn = __slope_legend_draw;
        first_call = SLOPE_FALSE;
    }

    return &klass;
}


slope_item_t* slope_legend_create ()
{
    slope_legend_t *legend = malloc(sizeof(slope_legend_t));
    slope_item_t *parent = (slope_item_t*) legend;

    parent->klass = __slope_legend_get_class();
    parent->visible = SLOPE_TRUE;

    slope_color_set_name(&legend->fill_color, SLOPE_WHITE);
    slope_color_set_name(&legend->stroke_color, SLOPE_BLACK);
    
    return parent;
}


void __slope_legend_draw (slope_item_t *legend, cairo_t *cr,
                          const slope_metrics_t *metrics)
{

}


void __slope_legend_eval_geometry (slope_item_t *legend,
                                   const slope_metrics_t *metrics)
{
    
}

/* slope/legend.c */

