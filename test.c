
#include <gtk/gtk.h>
#include <slope/slope.h>
#include <math.h>

#define N 500


static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    slope_rect_t rect;
    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) gtk_widget_get_allocated_width(widget);
    rect.height = (double) gtk_widget_get_allocated_height(widget);

    slope_scene_t *scene = (slope_scene_t*) data;
    slope_scene_draw(scene, cr, &rect);

    return TRUE;
}


int main(int argc, char *argv[])
{
    double x[N], y1[N], y2[N];
    int k;
    for (k=0; k<N; k++) {
        x[k] = (2.0*M_PI/N)*k;
        y1[k] = sin(x[k]);
        y2[k] = cos(x[k]);
    }

    slope_data_t *sin_data =
        slope_xydata_create_simple(
            x, y1, N, "Sine",
            SLOPE_BLUE, SLOPE_LINE);

    slope_data_t *cos_data =
        slope_xydata_create_simple(
            x, y2, N, "Cossine",
            SLOPE_RED, SLOPE_LINE);

    slope_metrics_t *metrics = slope_xymetrics_create();
    slope_metrics_add_data(metrics, sin_data);
    slope_metrics_add_data(metrics, cos_data);

    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_metrics(scene, metrics);

    gtk_init(&argc,&argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(window, 600, 450);
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(window), "delete-event",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
                     G_CALLBACK(on_draw), scene);
    gtk_widget_show_all(window);
    gtk_main();

    slope_scene_write_to_png(scene, "figure.png", 600, 450); 
    slope_data_destroy(sin_data);
    slope_data_destroy(cos_data);
    slope_metrics_destroy(metrics);
    slope_scene_destroy(scene);
    return 0;
}
