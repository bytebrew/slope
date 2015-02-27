
#include "slope.h"

int main()
{
    slope_scene_t *scene = slope_scene_create();
    slope_plotable_t *cartesian = slope_cartesian_create();
    slope_scene_add_plotable(scene, cartesian);
    slope_scene_write_to_png(scene, "figure.png", 500, 500);
    slope_plotable_destroy(cartesian);
    slope_scene_destroy(scene);
    return 0;
}
