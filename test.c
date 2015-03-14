
#include <gtk/gtk.h>
#include <slope/slope.h>
#include <math.h>

#define N 500


static gboolean on_draw(GtkWidget *widget,
                        cairo_t *cr, gpointer data)
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
    double x[N], y1[N], y2[N], y3[N];
    int k;
    for (k=0; k<N; k++) {
        x[k] = (2.0*M_PI/N)*k;
        y1[k] = sin(x[k]);
        y2[k] = y1[k] + 0.3*cos(4.0*x[k]);
        y3[k] = y1[k] + 0.3*sin(6.0*x[k]);
    }

    slope_data_t *model_data =
        slope_xydata_create_simple(
            x, y1, N, "Model",
            SLOPE_BLUE, SLOPE_LINE);

    slope_data_t *sample1_data =
        slope_xydata_create_simple(
            x, y2, N, "Sample1",
            SLOPE_GREEN, SLOPE_CIRCLES);
        
    slope_data_t *sample2_data =
        slope_xydata_create_simple(
            x, y3, N, "Sample2",
            SLOPE_RED, SLOPE_TRIANGLES);

    slope_metrics_t *metrics = slope_xymetrics_create();
    slope_metrics_add_data(metrics, model_data);
    slope_metrics_add_data(metrics, sample1_data);
    slope_metrics_add_data(metrics, sample2_data);

    slope_frame_t *frame = slope_xymetrics_get_frame(metrics);
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_TOP, "Samples and model");
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_BOTTOM, "Phase (RADIANS)");
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_LEFT, "Intensity");
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_RIGHT, "Intensity");

    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_metrics(scene, metrics);

    gtk_init(&argc,&argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 450);
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(window), "delete-event",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
                     G_CALLBACK(on_draw), scene);
    gtk_widget_show_all(window);
    gtk_main();

    slope_scene_write_to_png(scene, "figure.png", 450, 300);
    slope_data_destroy(model_data);
    slope_data_destroy(sample1_data);
    slope_data_destroy(sample2_data);
    slope_metrics_destroy(metrics);
    slope_scene_destroy(scene);
    return 0;
}
