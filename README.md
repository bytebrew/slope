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

* A quick plot of a data series of 2D points

![](https://github.com/elvismt/slope/blob/master/demos/scatter.png)

* An "area under" plot with custom axis labels

![](https://github.com/elvismt/slope/blob/master/demos/cos.png)

Mouse interactions like zooming are in the roadmap and will soon be added.
Contributions to slope are wellcome. Propose yours by issuing a pull request or
email me at elvismtt@gmail.com.
