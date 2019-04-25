#include "slope/figure.h"
#include "slope/color.h"
#include "slope/drawing.h"

typedef struct {
  SlopeRGBA back_color;
} SlopeFigurePrivate;


G_DEFINE_TYPE_WITH_PRIVATE (SlopeFigure, slope_figure, G_TYPE_OBJECT)

static void base_draw (SlopeFigure *self, SlopeDC *dc, const SlopeRect *rect);


static void
slope_figure_class_init (SlopeFigureClass *klass) {

    klass->draw = base_draw;
}


static void
slope_figure_init (SlopeFigure *self)
{
  SlopeFigurePrivate *priv = slope_figure_get_instance_private (self);

  priv->back_color = SLOPE_WHITE;
}


static void
slope_figure_dispose (GObject *gobject)
{
  SlopeFigurePrivate *priv = slope_figure_get_instance_private (SLOPE_FIGURE (gobject));
  G_OBJECT_CLASS (slope_figure_parent_class)->dispose (gobject);
}


static void
slope_figure_finalize (GObject *gobject)
{
  SlopeFigurePrivate *priv = slope_figure_get_instance_private (SLOPE_FIGURE (gobject));
  G_OBJECT_CLASS (slope_figure_parent_class)->finalize (gobject);
}


SlopeFigure *slope_figure_new(void) {
    return SLOPE_FIGURE (g_object_new(SLOPE_TYPE_FIGURE, NULL));
}


static void base_draw (SlopeFigure *self, SlopeDC *dc, const SlopeRect *rect) {
    SlopeFigurePrivate *priv = slope_figure_get_instance_private (SLOPE_FIGURE (self));

    /* fill background */
    if (slope_rgba_visible(priv->back_color)) {
        slope_set_color(dc->cr, priv->back_color);
        slope_rect(dc->cr, rect);
        cairo_fill(dc->cr);
    }
}


void slope_figure_draw (SlopeFigure *self, SlopeDC *dc, const SlopeRect *rect) {
    SLOPE_FIGURE_GET_CLASS(self)->draw(self, dc, rect);
}
