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

#include <slope/figure_p.h>
#include <slope/scale_p.h>


typedef struct
_SlopeFigurePrivate
{
    SlopeView *view;
    GList *scale_list;
    SlopeColor background_color;
    gboolean managed;

    double layout_rows;
    double layout_cols;
    int frame_mode;
}
SlopeFigurePrivate;


#define SLOPE_FIGURE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
     SLOPE_FIGURE_TYPE, SlopeFigurePrivate))

G_DEFINE_TYPE_WITH_PRIVATE(
    SlopeFigure, slope_figure, G_TYPE_OBJECT)


static void _figure_update_layout (SlopeFigure *self);
static void _figure_add_scale (SlopeFigure *self, SlopeScale *scale);
static void _figure_draw_frame (SlopeFigure *self, SlopeRect *rect,
                                const SlopeRect *in_rect, cairo_t *cr);
static void _figure_draw (SlopeFigure *self, const SlopeRect *rect, cairo_t *cr);
static void _figure_clear_scale_list (gpointer data);
static void _figure_finalize (GObject *self);



static void
slope_figure_class_init (SlopeFigureClass *klass)
{
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);

    object_klass->finalize = _figure_finalize;

    klass->add_scale = _figure_add_scale;
    klass->draw = _figure_draw;
}


static void
slope_figure_init (SlopeFigure *self)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    priv->view = NULL;
    priv->scale_list = NULL;
    priv->background_color = SLOPE_WHITE;
    priv->managed = TRUE;
    priv->frame_mode = SLOPE_FIGURE_ROUNDRECTANGLE;
}


static
void _figure_finalize (GObject *self)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    if (priv->scale_list != NULL) {
        g_list_free_full(priv->scale_list, _figure_clear_scale_list);
        priv->scale_list = NULL;
    }

    G_OBJECT_CLASS(slope_figure_parent_class)->finalize(self);
}


SlopeFigure* slope_figure_new ()
{
    SlopeFigure *self = SLOPE_FIGURE(g_object_new(SLOPE_FIGURE_TYPE, NULL));

    return self;
}


static
void _figure_add_scale (SlopeFigure *self, SlopeScale *scale)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    if (scale == NULL) {
        return;
    }

    priv->scale_list = g_list_append(priv->scale_list, scale);
    _scale_set_figure(scale, self);
    slope_scale_rescale(scale);
    _figure_update_layout(self);
}


static void _figure_draw_frame (SlopeFigure *self, SlopeRect *rect,
                                const SlopeRect *in_rect, cairo_t *cr)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);

    if (priv->frame_mode == SLOPE_FIGURE_ROUNDRECTANGLE) {
        rect->x = in_rect->x + 10.0;
        rect->y = in_rect->y + 10.0;
        rect->width = in_rect->width - 20.0;
        rect->height = in_rect->height - 20.0;
        slope_cairo_round_rect(cr, rect, 10.0);
    }
    else {
        *rect = *in_rect;
        slope_cairo_rect(cr, rect);
    }
}


static
void _figure_draw (SlopeFigure *self, const SlopeRect *in_rect, cairo_t *cr)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    double layout_cell_width, layout_cell_height;
    SlopeRect rect;
    GList *iter;

    /* save cr's state and clip tho the figure's rectangle,
       fill the background if required */
    cairo_save(cr);
    cairo_select_font_face(cr, "Sans",
          CAIRO_FONT_SLANT_NORMAL,
          CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    cairo_new_path(cr);
    _figure_draw_frame(self, &rect, in_rect, cr);
    if (!SLOPE_COLOR_IS_NULL(priv->background_color)) {
        slope_cairo_set_color(cr, priv->background_color);
        cairo_fill_preserve(cr);
    }
    cairo_clip(cr);


    layout_cell_width = rect.width / priv->layout_cols;
    layout_cell_height = rect.height / priv->layout_rows;
    iter = priv->scale_list;

    while (iter != NULL) {
        SlopeScale *scale = SLOPE_SCALE(iter->data);
        if (slope_scale_get_is_visible(scale) == TRUE) {
            SlopeRect scale_rect, layout;
            slope_scale_get_layout_rect(scale, &scale_rect);

            layout.x = rect.x + scale_rect.x * layout_cell_width;
            layout.y = rect.y + scale_rect.y * layout_cell_height;
            layout.width = scale_rect.width * layout_cell_width;
            layout.height = scale_rect.height * layout_cell_height;

            _scale_draw(scale, &layout, cr);
        }

        iter = iter->next;
    }

    /* give back cr in the same state as we received it */
    cairo_restore(cr);
}


static
void _figure_clear_scale_list (gpointer data)
{
    if (slope_scale_get_is_managed(SLOPE_SCALE(data)) == TRUE) {
        g_object_unref(G_OBJECT(data));
    }
}


void _figure_set_view (SlopeFigure *self, SlopeView *view)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    GList *iter;

    priv->view = view;

    iter = priv->scale_list;
    while (iter != NULL) {
        SlopeScale *scale = SLOPE_SCALE(iter->data);
        _scale_set_figure(scale, self);
        iter = iter->next;
    }
}


void slope_figure_write_to_png (SlopeFigure *self, const char *filename,
                               int width, int height)
{
    SlopeFigurePrivate *priv;
    cairo_surface_t *image;
    cairo_t *cr;
    SlopeRect rect;
    int mode_back;

    if (filename == NULL || width <= 0 || height <= 0) {
        return;
    }

    priv = SLOPE_FIGURE_GET_PRIVATE(self);
    image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(image);

    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = width;
    rect.height = height;

    mode_back = priv->frame_mode;
    priv->frame_mode = SLOPE_FIGURE_RECTANGLE;
    slope_figure_draw(self, &rect, cr);
    cairo_surface_write_to_png(image, filename);
    priv->frame_mode = mode_back;

    cairo_surface_destroy(image);
    cairo_destroy(cr);
}


static
void _figure_update_layout (SlopeFigure *self)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    GList *iter;

    priv->layout_rows = 0.0;
    priv->layout_cols = 0.0;

    iter = priv->scale_list;
    while (iter != NULL) {
        SlopeRect scale_rect;
        slope_scale_get_layout_rect(SLOPE_SCALE(iter->data), &scale_rect);

        if (scale_rect.x + scale_rect.width > priv->layout_cols) {
            priv->layout_cols = scale_rect.x + scale_rect.width;
        }

        if (scale_rect.y + scale_rect.height > priv->layout_rows) {
            priv->layout_rows = scale_rect.y + scale_rect.height;
        }

        iter = iter->next;
    }
}


void _figure_handle_mouse_event (SlopeFigure *self, SlopeViewMouseEvent *event)
{
    SlopeFigurePrivate *priv = SLOPE_FIGURE_GET_PRIVATE(self);
    GList *iter;

    /* delegate the handling of the event down to the scales and it's
       items */
    iter = priv->scale_list;
    while (iter != NULL) {
        SlopeScale *scale = SLOPE_SCALE(iter->data);

        if (_scale_handle_mouse_event(scale, event) == TRUE) {
            return;
        }

        iter = iter->next;
    }
}


GList* slope_figure_get_scale_list (SlopeFigure *self)
{
    return SLOPE_FIGURE_GET_PRIVATE(self)->scale_list;
}


SlopeColor slope_figure_get_background_color (SlopeFigure *self)
{
    return SLOPE_FIGURE_GET_PRIVATE(self)->background_color;
}


void slope_figure_set_background_color (SlopeFigure *self, SlopeColor color)
{
    SLOPE_FIGURE_GET_PRIVATE(self)->background_color = color;
}


SlopeView* slope_figure_get_view (SlopeFigure *self)
{
    return SLOPE_FIGURE_GET_PRIVATE(self)->view;
}


gboolean slope_figure_get_is_managed (SlopeFigure *self)
{
    return SLOPE_FIGURE_GET_PRIVATE(self)->managed;
}


void slope_figure_set_is_managed (SlopeFigure *self, gboolean managed)
{
    SLOPE_FIGURE_GET_PRIVATE(self)->managed = managed;
}


void slope_figure_draw (SlopeFigure *self, const SlopeRect *rect, cairo_t *cr)
{
    SLOPE_FIGURE_GET_CLASS(self)->draw(self, rect, cr);
}


void slope_figure_add_scale (SlopeFigure *self, SlopeScale *scale)
{
    SLOPE_FIGURE_GET_CLASS(self)->add_scale(self, scale);
}

/* slope/figure.c */
