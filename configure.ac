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

m4_define([slope_major_version], [2])
m4_define([slope_minor_version], [6])
m4_define([slope_patch_version], [0])
m4_define([slope_version],
      [slope_major_version.slope_minor_version.slope_patch_version])

AC_INIT([slope], [slope_version])

# Will be generated
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign no-dependencies subdir-objects])
AC_PROG_CC
AM_PROG_AR
LT_INIT
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# define version via command line
AX_APPEND_FLAG(
   [-DSLOPE_MAJOR_VERSION=slope_major_version])
AX_APPEND_FLAG(
   [-DSLOPE_MINOR_VERSION=slope_minor_version])
AX_APPEND_FLAG(
   [-DSLOPE_PATCH_VERSION=slope_patch_version])
AX_APPEND_FLAG(
   [-DSLOPE_VERSION_STRING=slope_version])

# check for gtk
PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.0])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([slope/Makefile])
AC_CONFIG_FILES([slope/source/Makefile])
AC_CONFIG_FILES([slope/source/slope/Makefile])
AC_CONFIG_FILES([slope/include/Makefile])
AC_CONFIG_FILES([slope/include/slope/Makefile])
AC_CONFIG_FILES([demos/Makefile])

AC_OUTPUT
