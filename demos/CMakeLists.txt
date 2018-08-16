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

# Collect cpp files and for each of them
# create a build target linking against the
# main metis library
file(GLOB_RECURSE DemoSources "*.c")
foreach(FileName ${DemoSources})
    get_filename_component(DemoName ${FileName} NAME_WE)
    add_executable(${DemoName} ${FileName})
    target_link_libraries(${DemoName} slope ${GTK_LIBRARIES} -lm)
    target_include_directories(
        ${DemoName} PRIVATE
        ${SLOPE_BASE_DIR}/slope/include
        ${SLOPE_AUTOGEN_DIR}
        ${GTK_INCLUDE_DIRS}
    )
endforeach()
