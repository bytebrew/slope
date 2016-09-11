
#include <slope/slope.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *view;
    SlopeScene *scene;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    view = slope_view_new();
    scene = slope_scene_new();

    gtk_container_add(GTK_CONTAINER(window), view);
    slope_view_set_scene(SLOPE_VIEW(view), scene);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    slope_scene_write_to_png(scene, "figure.png", 600, 450);
    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(scene);
    return 0;
}
