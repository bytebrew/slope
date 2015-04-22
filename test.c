
#include <gtk/gtk.h>
#include <slope/slope.h>
#include <math.h>

#define N 500

static void create_window(slope_scene_t *scene);
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data);


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

    /* this is all you need to know to crete a chart */
    slope_scene_t *chart = slope_chart_create("Trigonometry", "X", "Y");
    slope_chart_add_plot(chart, x, y1, N, "Sine", "r-");
    slope_chart_add_plot(chart, x, y2, N, "Cossine", "b+");

    /* put the scene in a gtk widget */
    gtk_init(&argc,&argv);
    create_window(chart);
    gtk_main();

    slope_scene_write_to_png(chart, "figure.png", 500, 350);
    slope_chart_destroy(chart);
    return 0;
}



static void create_window(slope_scene_t *scene)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_widget_set_size_request(window, 200, 200);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(window), "delete-event",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
                     G_CALLBACK(on_draw), scene);
    gtk_widget_show_all(window);
}


static gboolean on_draw(GtkWidget *widget,
                        cairo_t *cr,
                        gpointer data)
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

