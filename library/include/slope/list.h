/*
 * Copyright (C) 2018  Elvis Teixeira
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

#ifndef SLOPE_LIST_H
#define SLOPE_LIST_H

#include "slope/global.h"

typedef struct _SlpList SlpList;
#define SLP_LIST(Addr) ((SlpList*) (Addr))

struct _SlpList {
    SlpList *front;
    SlpList *back;
};

#define SLP_LIST_INIT(L) \
    G_STMT_START { \
        SLP_LIST(L)->front = NULL; \
        SLP_LIST(L)->back = NULL; \
    } G_STMT_END

#endif /* SLOPE_LIST_H */
