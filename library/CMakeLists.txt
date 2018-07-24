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

file(GLOB_RECURSE LibslopeHeaders "./*.h")
file(GLOB_RECURSE LibslopeSources "./*.c")

add_library(
    slope SHARED
    "${LibslopeHeaders}"
    "${LibslopeSources}")

set_target_properties(
    slope PROPERTIES
    COMPILE_FLAGS "-fPIC")

target_include_directories(
    slope PRIVATE
    "${SLOPE_INCLUDE_DIR}"
    "${SLOPE_SOURCE_DIR}"
    "${SLOPE_AUTOGEN_INCLUDE_DIR}"
    "${SLOPE_AUTOGEN_SOURCE_DIR}")

target_compile_definitions(
    slope PRIVATE
    SLOPE_LIBRARY_COMPILATION)

install(
    TARGETS slope
    DESTINATION "/usr/lib")

install(
    DIRECTORY "${SLOPE_INCLUDE_DIR}/slope"
    DESTINATION "/usr/include")

install(
    FILES "${SLOPE_AUTOGEN_INCLUDE_DIR}/slope/platform.h"
    DESTINATION "/usr/include/slope")
