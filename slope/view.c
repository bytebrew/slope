#include "slope/view.h"

typedef struct {
  SlopeFigure *figure;
} SlopeViewPrivate;


G_DEFINE_TYPE_WITH_PRIVATE (SlopeView, slope_view, GTK_TYPE_DRAWING_AREA)


static void
slope_view_class_init (SlopeViewClass *klass)
{
}


static void
slope_view_init (SlopeView *self)
{
  SlopeViewPrivate *priv = slope_view_get_instance_private (self);

  priv->figure = slope_figure_new();
}


static void
slope_view_dispose (GObject *gobject)
{
  SlopeViewPrivate *priv = slope_view_get_instance_private (SLOPE_VIEW (gobject));

  g_object_unref (G_OBJECT (priv->figure));

  G_OBJECT_CLASS (slope_view_parent_class)->dispose (gobject);
}


static void
slope_view_finalize (GObject *gobject)
{
  SlopeViewPrivate *priv = slope_view_get_instance_private (SLOPE_VIEW (gobject));
  G_OBJECT_CLASS (slope_view_parent_class)->finalize (gobject);
}


GtkWidget *slope_view_new (void) {
    return GTK_WIDGET (g_object_new(SLOPE_TYPE_VIEW, NULL));
}


SlopeFigure *slope_view_get_figure (SlopeView *self) {
    SlopeViewPrivate *priv = slope_view_get_instance_private (self);
    return priv->figure;
}
