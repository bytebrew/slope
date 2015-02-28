
#include "slope.h"
#include <math.h>
#include <gtk/gtk.h>


static gboolean on_draw (GtkWidget *area, cairo_t *cr, gpointer data)
{
    int width = gtk_widget_get_allocated_width(area);
    int height = gtk_widget_get_allocated_height(area);
    slope_rect_t rect;
    slope_rect_set(&rect, 0, 0, width, height);
    slope_scene_t *scene = (slope_scene_t*) data;
    slope_scene_draw(scene,cr,&rect);
    return TRUE;
}

int main(int argc, char *argv[])
{
    #define N 100
    double x[N], y1[N], y2[N];
    int k;
    for (k=0; k<N; k++) {
        x[k] = 0.1*k;
        y1[k] = sin(x[k]);
        y1[k] = cos(x[k]);
    }
    
    slope_scatter_t *sin_scatter =
        slope_scatter_create_simple(
            x, y1, N, SLOPE_LINE, SLOPE_RED);
        
    slope_scatter_t *cos_scatter =
        slope_scatter_create_simple(
            x, y2, N, SLOPE_LINE, SLOPE_BLUE);
    
    slope_plotable_t *cartesian = slope_cartesian_create();
    slope_cartesian_add_scatter(cartesian, sin_scatter);
    slope_cartesian_add_scatter(cartesian, cos_scatter);
    
    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_plotable(scene, cartesian);
    
    gtk_init(&argc,&argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window),area);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 450);
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw), scene);
    gtk_widget_show_all(window);
    gtk_main();
    
    slope_scatter_destroy(sin_scatter);
    slope_scatter_destroy(cos_scatter);
    slope_plotable_destroy(cartesian);
    slope_scene_destroy(scene);
    return 0;
}
