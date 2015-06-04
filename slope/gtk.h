/*
 * Copyright (C) 2015  Elvis Teixeira
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
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLOPE_GTK_H
#define SLOPE_GTK_H

#include "slope/view.h"

SLOPE_BEGIN_DECLS

typedef struct _slope_window slope_window_t;

/**
 */
slope_public GtkWidget*
slope_create_window (slope_figure_t *figure,
                     const char *title);

SLOPE_END_DECLS

#endif //SLOPE_GTK_H
