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

#ifndef _slope_frame_h_
#define _slope_frame_h_

#include "slope/item.h"

G_BEGIN_DECLS

#define SLOPE_TYPE_FRAME                 (slope_frame_get_type ())
#define SLOPE_FRAME(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLOPE_TYPE_FRAME, SlopeFrame))
#define SLOPE_FRAME_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SLOPE_TYPE_FRAME, SlopeFrameClass))
#define SLOPE_IS_FRAME(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SLOPE_TYPE_FRAME))
#define SLOPE_IS_FRAME_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SLOPE_TYPE_FRAME))
#define SLOPE_FRAME_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SLOPE_TYPE_FRAME, SlopeFrameClass))

typedef struct _SlopeFrame             SlopeFrame;
typedef struct _SlopeFrameClass        SlopeFrameClass;


typedef enum {
    SLOPE_FRAME_DRAW_RECT    = (1U),
    SLOPE_FRAME_DRAW_TITLE   = (1U << 1U),
    SLOPE_FRAME_ROUND_RECT   = (1U << 2U),
} SlopeFrameOption;


struct _SlopeFrame
{
  /*< parent object space >*/
  SlopeItem parent;

  /*< private >*/
  gpointer _slope_reserved1;
  gpointer _slope_reserved2;
  gpointer _slope_reserved3;
  gpointer _slope_reserved4;
};


struct _SlopeFrameClass
{
  SlopeItemClass parent_class;

  /* Padding for future expansion */
  void (*_slope_reserved1) (void);
  void (*_slope_reserved2) (void);
  void (*_slope_reserved3) (void);
  void (*_slope_reserved4) (void);
};


GType slope_frame_get_type (void) G_GNUC_CONST;

SlopeItem* slope_frame_new (void);

void slope_frame_set_title (SlopeFrame *self, const gchar *title);

const gchar* slope_frame_get_title (SlopeFrame *self);

int slope_frame_get_margin (SlopeFrame *self);

void slope_frame_draw_rect (SlopeFrame *self, const SlopeItemDC *dc);

void slope_frame_draw_title (SlopeFrame *self, const SlopeItemDC *dc);

G_END_DECLS

#endif /* _slope_frame_h_ */
