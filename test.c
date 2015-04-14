
#include <gtk/gtk.h>
#include <slope/slope.h>
#include <math.h>

#define N 500


static gboolean on_draw(GtkWidget *widget,
                        GdkEvent *event,
                        gpointer data)
{
    slope_rect_t rect;
    rect.x = 0.0;
    rect.y = 0.0;
    rect.width = (double) widget->allocation.width;
    rect.height = (double) widget->allocation.height;
    cairo_t *cr = gdk_cairo_create(widget->window);
    slope_scene_t *scene = (slope_scene_t*) data;
    slope_scene_draw(scene, cr, &rect);
    cairo_destroy(cr);
    return TRUE;
}


int main(int argc, char *argv[])
{
    const double step = 2.0*M_PI/N;
    double x[N], y1[N], y2[N];
    int k;
    for (k=0; k<N; k++) {
        x[k] = k*step;
        y1[k] = sin(x[k]);
        y2[k] = cos(x[k]);
    }

    /* put the data arrays in the data objects */
    slope_data_t *sin_data =
        slope_xydata_create_simple(
            x, y1, N, "Sine",
            SLOPE_BLUE, SLOPE_PLUSSES);

    slope_data_t *cos_data =
        slope_xydata_create_simple(
            x, y2, N, "Cossine",
            SLOPE_RED, SLOPE_LINE);

    /* put the data objectsin a metrics and the metrics in a scene */
    slope_metrics_t *metrics = slope_xymetrics_create();
    slope_metrics_add_data(metrics, sin_data);
    slope_metrics_add_data(metrics, cos_data);
    
    /* set axis labels */
    slope_data_t *axis;
    axis = slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_TOP);
    slope_data_set_name(axis, "Trigonometric functions");
    axis = slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_BOTTOM);
    slope_data_set_name(axis, "Phase");
    axis = slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_LEFT);
    slope_data_set_name(axis, "Amplitude");
    axis = slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_RIGHT);
    slope_data_set_name(axis, ""); /* no label */
    
    /* create the where everything will be drawn */
    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_metrics(scene, metrics);

    /* put the scene in a gtk widget */
    gtk_init(&argc,&argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_widget_set_size_request(window, 200, 200);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(window), "delete-event",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "expose-event",
                     G_CALLBACK(on_draw), scene);
    gtk_widget_show_all(window);
    gtk_main();

    slope_scene_write_to_png(scene, "figure.png", 500, 350);
    slope_data_destroy(sin_data);
    slope_data_destroy(cos_data);
    slope_metrics_destroy(metrics);
    slope_scene_destroy(scene);
    return 0;
}
