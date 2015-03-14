SLOPE
=====

Slope is a free (LGPL) **C** library that creates charts from numerical data. It's
design principle is the following: **THE ONLY DEPENDENCY IS CAIRO**, and the **API**
is familiar to the users of cairo. Of course, since you just have to pass a
**cairo_t** to slope to generate a chart, it is easy to embed slope's charts
in a **GTK WIDGET**.

Slope is in it's early stages of development and still needs some optimization
and polishing. Some accessor methods to objects properties are also absent. But here's
what we already have: The following chart is the output of the test.c program in de
top level source repository.

![](https://github.com/exocode/slope/blob/master/figure.png)

Slope's basic usage is: create a **slope_scene_t**, add one or more **slope_metrics_t**
to it and add some **slope_data_t** to the metrics, then use function **slope_scene_draw()**
to stroke the resulting chart to a **cairo_surface_t** via **cairo_t**.
