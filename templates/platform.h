/*
 * Copyright (C) 2019  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLOPE_PLATFORM_H
#define SLOPE_PLATFORM_H
// clang-format off

/** Use like #if (SLOPE_VERSION >= SLOPE_VERSION_ENCODE(0, 1, 0)) */
#define SLOPE_VERSION_ENCODE(major, minor, patch) \
    (((major)<<16)|((minor)<<8)|(patch))

/** Integer representing the version of this slope code */
#define SLOPE_VERSION  SLOPE_VERSION_ENCODE \
    (SLOPE_VERSION_MAJOR, SLOPE_VERSION_MINOR, SLOPE_VERSION_PATCH)


#define SLOPE_VERSION_MAJOR      @SLOPE_VERSION_MAJOR@
#define SLOPE_VERSION_MINOR      @SLOPE_VERSION_MINOR@
#define SLOPE_VERSION_PATCH      @SLOPE_VERSION_PATCH@
#define SLOPE_VERSION_STRING    "@SLOPE_VERSION_STRING@"
#define SLOPE_VERSION_SIGNATURE  @SLOPE_VERSION_SIGNATURE@


#define SLOPE_PLATFORM      "@SLOPE_PLATFORM@"
#define SLOPE_OS_NAME       "@SLOPE_OS_NAME@"


#cmakedefine01 SLOPE_OS_LINUX
#cmakedefine01 SLOPE_OS_WINDOWS
#cmakedefine01 SLOPE_OS_MAC


#define SLOPE_CC_NAME    "@SLOPE_CC_NAME@"
#define SLOPE_CC_VERSION "@SLOPE_CC_VERSION@"


#cmakedefine01 SLOPE_CC_GCC
#cmakedefine01 SLOPE_CC_CLANG
#cmakedefine01 SLOPE_CC_MSVC


#cmakedefine01 SLOPE_DEBUG_ENABLE
#cmakedefine01 SLOPE_STATS_ENABLE

// clang-format on
#endif /* SLOPE_PLATFORM_H */
