#
# Copyright (C) 2017  Elvis Teixeira
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
#

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK "gtk+-3.0")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror -O2")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here
# to make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." ON)

configure_file(
   config.h.in
   ${SLOPE_AUTOGEN_DIR}/config.h
)

# Collects the source files recursively from this
# directory and use them to build the main shared library
# The reason why we put the headers as target dependencies
# is only to make IDES list them in the side panel
file(GLOB_RECURSE LibslopeHeaders "*.h")
file(GLOB_RECURSE LibslopeSources "*.c")
add_library(slope SHARED ${LibslopeHeaders} ${LibslopeSources})

target_compile_definitions(
  slope PRIVATE
  SLOPE_BUILDING_SHARED
)

target_include_directories(
    slope PRIVATE
    "include"
    "source"
    ${SLOPE_AUTOGEN_DIR}
    ${GTK_INCLUDE_DIRS}
)

# Install only the public headers (those not anding with _p.h)
# the non-public ones should be under the source directory
file(GLOB_RECURSE PublicHeaders "include/*.h")
install(TARGETS slope RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(FILES ${PublicHeaders} DESTINATION include/slope)
install(FILES ${SLOPE_AUTOGEN_DIR}/config.h DESTINATION include/slope)
