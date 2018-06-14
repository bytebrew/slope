[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](http://www.gnu.org/licenses/lgpl-3.0)
[![Version](https://img.shields.io/badge/version-0.0.5-red.svg)](http://github.com/bytebrew/slope)

# SLOPE

Slope is a free (LGPL) **C** data visualization library for the Gtk+ toolkit.

![](https://github.com/bytebrew/slope/blob/master/demos/screenshot.png)

Slope integrates with **Gtk+ 3.X** and all the drawing is made with **cairo**, so it doesn't
depend on nothing outside the Gtk stack.

## Instalation

Install the required development packages:
- On Debian/Ubuntu
```bash
sudo apt-get install build-essential cmake libgtk-3-dev
```
- On Fedora:
```bash
sudo dnf install gcc cmake gtk3-devel
```

Compile and install slope:
```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## Using the library

After the library is installed you can compile your applications adding the -lslope
flag to the compiler command line. An example for a simple program, like the ones in the
demos directory would be:

```bash
gcc simple.c -lslope -lm -o simple `pkg-config --cflags --libs gtk+-3.0`
```

## Roadmap

 - ~~Legend (done)~~
 - More elaborate plots e.g. like boxplots (planned)
 - Contours and variance matrices visualization (planned)

For examples of basic usage see the demos directory in the source tree.

If you want to try, just download or clone the repository and use CMake to build (yes, we
would appreciate if some one contribute automake scripts). If you want to contribute to Slope
feel free to issue a pull request. Your help and feedback will be appreciated.
