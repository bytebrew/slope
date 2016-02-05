SLOPE
=====

Slope is a free (LGPL) **C** library for creating charts using the cairo 2D drawing
library. As long as **Gtk+** uses cairo to render it's widgets, embedding slope's
charts in gtk is straitforward.

Our goal is producing beautiful, publication quality, and fast charts from numerical
data in a nice object-oriented fashion. We are in part inspired by Python's matplotlib,
but commited with the raw speed of C programs. We beleave Slope together with the
GNU Scientific Library (GSL) is the best replacement for the set (NumPy, SciPy, matplotlib)
in C's world.

Slope's only depdendency is cairo and you can export charts to any of cairo's
backends. There is an alternative build option that compiles a Gtk+ widget to
help you show slope's charts in your Gtk+ app. This build of course makes slope
gtk dependent. The cairo only build will always be present.

Slope is in it's early stages of development, but it is already able to produce
beautifull charts like the ones bellow. The code to draw these charts is in the
demos directory in the source tree.

* An "area under" plot with custom x axis labels

![](https://github.com/elvismt/slope/blob/master/demos/cos_demo.png)

* A slope figure showing the result of a linear regression made with gsl

![](https://github.com/elvismt/slope/blob/master/demos/regression_demo.png)

* A quick plot of a data series in a Windows(R) build (MinGW)

![](https://github.com/elvismt/slope/blob/master/demos/scatter_demo.png)

Contributions to slope are wellcome. Propose yours by issuing a pull request or
email me at elvismtt@gmail.com.
