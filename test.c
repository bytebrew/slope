
#include "slope.h"
#include <gtk/gtk.h>
#include <math.h>

#define N 100


gboolean on_draw(GtkWidget *w, cairo_t *cr, gpointer data)
{
    slope_rect_t rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = gtk_widget_get_allocated_width(w);
    rect.height = gtk_widget_get_allocated_height(w);
    slope_scene_t *scene = (slope_scene_t*) data;
    slope_scene_draw(scene, cr, &rect);
    return TRUE;
}

int main(int argc, char *argv[])
{
    double x1[N], y1[N];
    double x2[N], y2[N];
    int k;
    for (k=0; k<N; k++) {
        x1[k] = 0.1*k;
        y1[k] = sin(x1[k]);
        x2[k] = 0.1*k;
        y2[k] = cos(x2[k]);
    }
    
    slope_xyplot_t *sin_plot =
        slope_xyplot_create_simple(
            x1, y1, N, SLOPE_RED, SLOPE_LINE);
    
    slope_xyplot_t *cos_plot =
        slope_xyplot_create_simple(
            x2, y2, N, SLOPE_BLUE, SLOPE_LINE);
    
    slope_metrics_t *cartesian = slope_cartesian_create();
    slope_cartesian_add_xyplot(cartesian, sin_plot);
    slope_cartesian_add_xyplot(cartesian, cos_plot);
    
    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_metrics(scene, cartesian);
    
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *area = gtk_drawing_area_new();
    gtk_window_set_default_size(GTK_WINDOW(window), 600,450);
    gtk_container_add(GTK_CONTAINER(window), area);
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw), scene);
    
    gtk_widget_show_all(window);
    gtk_main();
    
    slope_scene_write_to_png(scene, "figure.png", 600, 450);
    slope_xyplot_destroy(sin_plot);
    slope_xyplot_destroy(cos_plot);
    slope_metrics_destroy(cartesian);
    slope_scene_destroy(scene);
    return 0;
}
