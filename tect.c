
#include "slope-chart.h"

int main()
{
    slope_chart_t *chart = slope_chart_create();
    slope_chart_set_back_color_by_name(chart, SLOPE_BLACK);
    slope_chart_write_to_png(chart, "figure.png", 500, 500);
    slope_chart_destroy(chart);
    return 0;
}
