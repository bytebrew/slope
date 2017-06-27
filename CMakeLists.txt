project(slope)
cmake_minimum_required(VERSION 2.8)

set(SLOPE_MAJOR_VERSION 0)
set(SLOPE_MINOR_VERSION 8)
set(SLOPE_PATCH_VERSION 0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK "gtk+-3.0")
  
subdirs(
   "${CMAKE_SOURCE_DIR}/src"
   "${CMAKE_SOURCE_DIR}/demos"
)

