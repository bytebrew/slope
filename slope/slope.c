
#include "slope/slope.h"
#include <stdlib.h>

slope_scene_t* slope_chart_create (const char *title,
                                   const char *xlabel,
                                   const char *ylabel)
{
    slope_scene_t *scene = slope_scene_create();
    slope_metrics_t *metrics = slope_xymetrics_create();
    slope_data_set_name(
        slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_TOP), title);
    slope_data_set_name(
        slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_BOTTOM), xlabel);
    slope_data_set_name(
        slope_xymetrics_get_axis(metrics, SLOPE_XYAXIS_LEFT), ylabel);
    slope_scene_add_metrics(scene, metrics);
    return scene;
}


void slope_chart_destroy (slope_scene_t *scene)
{
    if (scene == NULL) {
        return;
    }
    slope_iterator_t *met_iter =
        slope_list_first(slope_scene_get_metrics_list(scene));
    while (met_iter) {
        slope_metrics_t *metrics =
            (slope_metrics_t*) slope_iterator_data(met_iter);
        slope_iterator_t *plot_iter =
            slope_list_first(slope_metrics_get_data_list(metrics));
        while (plot_iter) {
            slope_data_t *plot =
                (slope_data_t*) slope_iterator_data(plot_iter);
            slope_data_destroy(plot);
            slope_iterator_next(&plot_iter);
        }
        slope_metrics_destroy(metrics);
        slope_iterator_next(&met_iter);
    }
    slope_scene_destroy(scene);
}


void slope_chart_add_plot (slope_scene_t *chart,
                           const double *x, const double *y, int n,
                           const char *title, int line,const char *fmt)
{
    slope_data_t *plot =
        slope_xydata_create_simple(x, y, n, title, line, fmt);
    slope_iterator_t *iter =
        slope_list_first(slope_scene_get_metrics_list(chart));
    slope_metrics_t *metrics =
        (slope_metrics_t*) slope_iterator_data(iter);
    slope_metrics_add_data(metrics, plot);
}

/* slope/slope.h */

