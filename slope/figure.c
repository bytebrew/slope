#include "slope/figure.h"

typedef struct {
  gchar *filename;
  /* stuff */
} SlopeFigurePrivate;


G_DEFINE_TYPE_WITH_PRIVATE (SlopeFigure, slope_figure, G_TYPE_OBJECT)


static void
slope_figure_class_init (SlopeFigureClass *klass)
{
}


static void
slope_figure_init (SlopeFigure *self)
{
  SlopeFigurePrivate *priv = slope_figure_get_instance_private (self);
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
