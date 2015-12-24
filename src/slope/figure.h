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

#ifndef __SLOPE_FIGURE__
#define __SLOPE_FIGURE__

#include <slope/drawing.h>
#include <slope/list.h>
#include <slope/legend.h>

#define SLOPE_FIGURE(instance) ((slope_figure_t*) (instance))
#define SLOPE_FIGURE_CLASS(instance) ((slope_figure_class_t*) (instance))
#define SLOPE_FIGURE_GET_CLASS(instance) SLOPE_FIGURE_CLASS(SLOPE_FIGURE(instance)->_class)

SLOPE_BEGIN_DECLS

struct _slope_figure_class {
    void (*init) (slope_figure_t*);
    void (*finalize) (slope_figure_t*);
    void (*draw) (slope_figure_t*, const slope_rect_t*, cairo_t*);
    void (*set_color_scheme) (slope_figure_t*, slope_color_t, slope_color_t, slope_color_t);
};


struct _slope_figure {
   struct _slope_figure_class *_class;
   void *_private;
};


slope_figure_t* slope_figure_new (const char *name);
void slope_figure_destroy (slope_figure_t *self);
void slope_figure_init (slope_figure_t *self);
slope_list_t* slope_figure_get_scale_list (const slope_figure_t *self);
slope_scale_t* slope_figure_get_reference_scale (const slope_figure_t *self);
void slope_figure_finalize (slope_figure_t *self);
slope_item_t* slope_figure_get_legend (const slope_figure_t *self);
void slope_figure_set_name (slope_figure_t *self, const char *name);
const char* slope_figure_get_name (const slope_figure_t *self);
void slope_figure_get_rect (const slope_figure_t *self, slope_rect_t *rect);
void slope_figure_add_scale (slope_figure_t *self, slope_scale_t *scale);
void slope_figure_draw (slope_figure_t *self, const slope_rect_t *rect, cairo_t *cr);
void slope_figure_write_to_png (slope_figure_t *self, const char *filename, int width, int height);
void slope_figure_set_color_scheme (slope_figure_t *self, slope_color_t background,
                                    slope_color_t foreground, slope_color_t extra_color);

SLOPE_END_DECLS

#endif /*__SLOPE_FIGURE__*/
