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

#ifndef SLOPE_OBJECT_H
#define SLOPE_OBJECT_H

#include "slope/global.h"

SLOPE_BEGIN_DECLS

typedef struct _slope_object         slope_object_t;
typedef struct _slope_object_class   slope_object_class_t;
typedef struct _slope_object_private slope_object_private_t;


struct _slope_object
{
  slope_object_class_t *klass;
  slope_object_private_t *priv;
};


struct _slope_object_class
{
    void (*init_fn) (slope_object_t *self);
  void (*destroy_fn) (slope_object_t *self);
};


slope_public void
slope_object_destroy (slope_object_t *self);


SLOPE_END_DECLS

#endif /* SLOPE_OBJECT_H */
