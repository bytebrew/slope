
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
    double x[N], y1[N], y2[N];
    int k;
    for (k=0; k<N; k++) {
        x[k] = -5.0 + (10.0/N)*k;
        y1[k] = exp(-x[k]*x[k]);
        y2[k] = y1[k] + 0.2*y1[k]*cos(10.0*x[k]);
    }
    
    /* put the data arrays in the data objects */
    slope_data_t *model_data =
        slope_xydata_create_simple(
            x, y1, N, "Model",
            SLOPE_BLUE, SLOPE_LINE);
    
    slope_data_t *sample_data =
        slope_xydata_create_simple(
            x, y2, N, "Sample",
            SLOPE_RED, SLOPE_CIRCLES);
    
    /*
     *create a metrics to place the data in the chart */
    slope_metrics_t *metrics = slope_xymetrics_create();
    slope_metrics_add_data(metrics, sample_data);
    slope_metrics_add_data(metrics, model_data);
    
    /*
     * this is how you set axis labels
     */
    slope_frame_t *frame = slope_xymetrics_get_frame(metrics);
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_TOP, "Samples and Model");
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_BOTTOM, "Phase");
    slope_xyframe_set_label(frame, SLOPE_XYFRAME_LEFT, "Intensity");
    
    /*
     *add the metrics to a scene */
    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_metrics(scene, metrics);
    
    /*
     * this is how you control axis visibility
     slope_xyframe_set_visible(frame, SLOPE_XYFRAME_TOP, SLOPE_FALSE);
     slope_xyframe_set_visible(frame, SLOPE_XYFRAME_RIGHT, SLOPE_FALSE);
     */
    
    /*
     * this is how you control scene and axis colour
     slope_scene_set_back_color_name(scene, SLOPE_BLACK);
     slope_xyframe_set_color_name(frame, SLOPE_WHITE);
     */
    
    /*
     *put the scene in a gtk widget */
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
    
    slope_scene_write_to_png(scene, "figure.png", 500, 350);
    slope_data_destroy(model_data);
    slope_data_destroy(sample_data);
    slope_metrics_destroy(metrics);
    slope_scene_destroy(scene);
    return 0;
}
