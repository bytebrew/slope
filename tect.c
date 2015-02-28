
#include "slope.h"
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
    gtk_init(&argc,&argv);
    slope_scene_t *scene = slope_scene_create();
    slope_plotable_t *cartesian = slope_cartesian_create();
    slope_scene_add_plotable(scene, cartesian);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window),area);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 450);
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw), scene);
    gtk_widget_show_all(window);
    gtk_main();
    slope_plotable_destroy(cartesian);
    slope_scene_destroy(scene);
    return 0;
}
