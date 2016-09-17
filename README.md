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

Slope, in it's GObject based reincarnation in it's early stages of development.
However it can already produce good charts like the one below. Give it a try.

![](https://github.com/elvismt/slope/blob/gobject/demos/screenshot.png)

