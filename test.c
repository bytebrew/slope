
#include <slope/slope.h>
#include <math.h>

#define N 100

int main()
{
    double x1[N], y1[N];
    int k;
    for (k=0; k<N; k++) {
        x1[k] = 0.1*k;
        y1[k] = sin(x1[k]);
    }
    
    slope_data_t *sin_data =
        slope_xydata_create_simple(
            x1, y1, N, "Sine",
            SLOPE_BLUE, SLOPE_LINE);
    
    slope_metrics_t *metrics = slope_xymetrics_create();
    slope_metrics_add_data(metrics, sin_data);
    
    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_metrics(scene, metrics);
    
    slope_scene_write_to_png(scene, "figure.png", 500, 500);
    
    slope_data_destroy(sin_data);
    slope_metrics_destroy(metrics);
    slope_scene_destroy(scene);
    return 0;
}
