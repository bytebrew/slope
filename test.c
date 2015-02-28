
#include "slope.h"
#include <math.h>

#define N 100

int main(int argc, char *argv[])
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
    
    slope_scatter_t *sin_scatter =
        slope_scatter_create_simple(
            x1, y1, N, SLOPE_RED, SLOPE_LINE);
    
    slope_scatter_t *cos_scatter =
        slope_scatter_create_simple(
            x2, y2, N, SLOPE_BLUE, SLOPE_LINE);
    
    slope_plotable_t *cartesian = slope_cartesian_create();
    slope_cartesian_add_scatter(cartesian, sin_scatter);
    slope_cartesian_add_scatter(cartesian, cos_scatter);
    
    slope_scene_t *scene = slope_scene_create();
    slope_scene_add_plotable(scene, cartesian);
    slope_scene_write_to_png(scene, "figure.png", 600, 450);
    
    slope_scatter_destroy(sin_scatter);
    slope_scatter_destroy(cos_scatter);
    slope_plotable_destroy(cartesian);
    slope_scene_destroy(scene);
    return 0;
}
