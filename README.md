SLOPE
=====

Slope is a free (LGPL) **C** library for creating charts using the cairo 2D drawing
library. As long as **Gtk+** uses cairo to render it's widgets, embedding slope's
charts in gtk is straitforward.

Slope's only depdendency is cairo and you can export charts to any of cairo's
backends. There is an alternative build option that compiles a Gtk+ widget to
help you show slope's charts in your Gtk+ app. This build of course makes slope
gtk dependent. The cairo only build will always be present.

If you prefer C++ and Qt use Kdv: https://github.com/elvismt/Kdv.

Slope is in it's early stages of development, but it is already able to produce
beautifull charts like the ones bellow. The code to draw these charts is in the
demos directory in the source tree.

* An "area under" plot with custom x axis labels

![](https://github.com/elvismt/slope/blob/master/demos/cos_demo.png)

* A demo from a Windows build

![](https://github.com/elvismt/slope/blob/master/demos/log_demo.png)

Contributions to slope are wellcome. Propose yours by issuing a pull request or
to email alvismtt@gmail.com.