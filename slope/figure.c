#include "slope/figure_p.h"
#include "slope/color.h"
#include "slope/drawing.h"


typedef struct {
  SlopeRGBA back_color;
} SlopeFigurePrivate;


G_DEFINE_TYPE_WITH_PRIVATE (SlopeFigure, slope_figure, G_TYPE_OBJECT)

static void figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect);
static void figure_dispose (GObject *gobject);
static void figure_init_dc (SlopeFigure *self, SlopeDC *dc, cairo_t *cr);

static void
slope_figure_class_init (SlopeFigureClass *klass) {
    GObjectClass *object_klass = G_OBJECT_CLASS(klass);
    klass->draw = figure_draw;
    object_klass->dispose = figure_dispose;
}


static void
slope_figure_init (SlopeFigure *self) {
  SlopeFigurePrivate *priv = slope_figure_get_instance_private (self);

  priv->back_color = SLOPE_WHITE;
}


static void
figure_dispose (GObject *gobject)
{
  /* SlopeFigurePrivate *priv = slope_figure_get_instance_private (SLOPE_FIGURE (gobject)); */
  G_OBJECT_CLASS (slope_figure_parent_class)->dispose (gobject);
}


SlopeFigure*
slope_figure_new(void) {
    return SLOPE_FIGURE (g_object_new(SLOPE_TYPE_FIGURE, NULL));
}


static void
figure_init_dc (SlopeFigure *self, SlopeDC *dc, cairo_t *cr) {
    dc->cr = cr;
}


static void
figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect) {
    SlopeFigurePrivate *priv = slope_figure_get_instance_private (SLOPE_FIGURE (self));

    SlopeDC dc;
    figure_init_dc(self, &dc, cr);

    /* fill background */
    if (slope_rgba_visible(priv->back_color)) {
        slope_set_color(cr, priv->back_color);
        slope_rect(cr, rect);
        cairo_fill(cr);
    }
}


void
slope_figure_draw (SlopeFigure *self, cairo_t *cr, const SlopeRect *rect) {
    SLOPE_FIGURE_GET_CLASS(self)->draw(self, cr, rect);
}
