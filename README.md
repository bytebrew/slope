# SLOPE

Slope is a free (LGPL) **C** library for data visualization.

![](https://github.com/elvismt/slope/blob/master/demos/screenshot.png)

Slope integrates with **Gtk+** and all the drawing is made with **cairo**, so it doesn't
depend on nothing outside Gtk.

## Instalation

Install the required development packages:

- In debian or it's derivaties:
```bash
sudo apt-get install build-essential cmake libgtk-3-dev
```

- In redhat or it's derivaties:
```bash
sudo dnf install gcc cmake gtk-devel
```

Then build and install slope by going into the source directory and typing
```bash
cmake ..
make
sudo make install
```

For examples of basic usage see the demos directory in the source tree.

If you want to try, just download or clone the repository and use CMake to build (yes, we
would appreciate if some one contribute automake scripts). If you want to contribute to Slope
feel free to issue a pull request or suggest something directly to me at elvismtt@gmail.com.
