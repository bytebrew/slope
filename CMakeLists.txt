project(slope C)
cmake_minimum_required(VERSION 2.8)

set(SLOPE_USE_GTK "TRUE")

find_package(PkgConfig REQUIRED)
if(SLOPE_USE_GTK MATCHES "TRUE")
    set(SLOPE_HAVE_GTK 1)
    PKG_CHECK_MODULES(DEP "gtk+-3.0")
else()
    set(SLOPE_HAVE_GTK 0)
    PKG_CHECK_MODULES(DEP "cairo")
endif()

include_directories(
    "${DEP_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/src"
)

subdirs(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/demos"
)
