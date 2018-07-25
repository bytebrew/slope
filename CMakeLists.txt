# Copyright (C) 2018  Elvis Teixeira
#
# This source code is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General
# Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any
# later version.
#
# This source code is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with this program.
# If not, see <http://www.gnu.org/licenses/>.

project(slope C)
cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)

set(SLOPE_VERSION_MAJOR  0)
set(SLOPE_VERSION_MINOR  1)
set(SLOPE_VERSION_PATCH  5)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK "gtk+-3.0")

string(CONCAT SLOPE_VERSION_STRING
    "${SLOPE_VERSION_MAJOR}"
    ".${SLOPE_VERSION_MINOR}"
    ".${SLOPE_VERSION_PATCH}")

include(cmake/settings.cmake)
include(cmake/platform.cmake)

add_subdirectory(library)
