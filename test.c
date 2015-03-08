
#include "slope.h"
#include <math.h>

#define N 100

int main()
{
    double x1[N], y1[N];
    double x2[N], y2[N];
    int k;
    for (k=0; k<N; k++) {
        x1[k] = 0.1*k;
        y1[k] = sin(x1[k]);
        x2[k] = 0.1*k;
        y2[k] = cos(x2[k]);
    }
    
    slope_scene_t *chart = slope_xychart_create();
    slope_xychart_add_plot(chart, x1, y1, N, SLOPE_RED, SLOPE_LINE);
    slope_xychart_add_plot(chart, x2, y2, N, SLOPE_BLUE, SLOPE_LINE);
    slope_scene_write_to_png(chart, "figure.png", 600, 450);
    slope_scene_destroy(chart);
    return 0;
}
