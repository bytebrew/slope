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

#include "slope/eps_pen_p.h"

slope_pen_class_t slope_epspen_class = {0U,
                                        slope_epspen_destroy,
                                        slope_epspen_begin_path,
                                        slope_epspen_end_path,
                                        slope_epspen_move_to,
                                        slope_epspen_line_to,
                                        slope_epspen_set_colors,
                                        slope_epspen_close_path,
                                        slope_epspen_circle,
                                        slope_epspen_set_width,
                                        slope_epspen_text};

slope_pen_t *slope_epspen_new(const char *   file_name,
                              slope_float_t  width,
                              slope_float_t  height,
                              slope_uint32_t opts)
{
  /*
   * Allocate memory for the pen object
   */
  slope_epspen_t *self = slope_new(slope_epspen_t);
  if (self == NULL) goto FAIL_alloc_pen;
  self->base.klass = &slope_epspen_class;

  /*
   * Open output file if required, otherwise use STDOUT
   */
  if (file_name)
    {
      self->file_name = strdup(file_name);
      if ((self->file = fopen(file_name, "w")) == NULL) goto FAIL_open_file;
    }
  else
    {
      self->file_name = NULL;
      self->file      = stdout;
    }

  /*
   * Initialize pen data members
   */
  self->pos.x        = 0.0;
  self->pos.y        = 0.0;
  self->fill_color   = SLOPE_NO_COLOR;
  self->stroke_color = SLOPE_BLACK;
  self->stroke_width = 2.0;
  self->opts         = opts;

  /*
   * Write file headings
   */
  slope_epspen_initialize_file(self, width, height);

  /*
   * Return new object of handle errors
   */
  return (slope_pen_t *) self;
FAIL_open_file:
  slope_free(self);
FAIL_alloc_pen:
  return NULL;
}

void slope_epspen_initialize_file(slope_epspen_t *self,
                                  slope_float_t   width,
                                  slope_float_t   height)
{
  fprintf(self->file, "\n");
}

void slope_epspen_finalize_file(slope_epspen_t *self)
{
  fprintf(self->file, "\n");
}

void slope_epspen_destroy(slope_pen_t *self)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  /*
   * Write file ending tags
   */
  slope_epspen_finalize_file(eps);

  /*
   * Close file and free file name
   */
  if (eps->file_name) slope_free(eps->file_name);
  fclose(eps->file);

  /*
   * Free pen object memory
   */
  slope_free(eps);
}

void slope_epspen_begin_path(slope_pen_t *self)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (slope_enabled(eps->opts, SLOPE_EPS_OP_IN_PROGRESS))
    {
      /* TODO report error */
      return;
    }

  /* let everyone know we are drawing a path */
  slope_enable(eps->opts, SLOPE_EPS_INSIDE_PATH);

  fprintf(eps->file, "newpath\n");
}

void slope_epspen_end_path(slope_pen_t *self)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (!slope_enabled(eps->opts, SLOPE_EPS_INSIDE_PATH))
    {
      /* TODO report error */
      return;
    }

  /* let everyone know we are done drawing the path */
  slope_disable(eps->opts, SLOPE_EPS_INSIDE_PATH);

  if (slope_color_is_visible(eps->stroke_color) &&
      slope_color_is_visible(eps->fill_color))
    {
      fprintf(eps->file, "%.1lf setlinewidth\n", eps->stroke_width);
      fprintf(eps->file, "gsave\n");
      fprintf(eps->file, "fill\n");
      fprintf(eps->file, "grestore\n");
      fprintf(eps->file, "stroke\n");
    }
  else if (slope_color_is_visible(eps->stroke_color))
    {
      fprintf(eps->file, "%.1lf setlinewidth\n", eps->stroke_width);
      fprintf(eps->file, "stroke\n");
    }
  else if (slope_color_is_visible(eps->fill_color))
    {
      fprintf(eps->file, "%.1lf setlinewidth\n", eps->stroke_width);
      fprintf(eps->file, "fill\n");
    }
}

void slope_epspen_move_to(slope_pen_t *self, slope_float_t x, slope_float_t y)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (!slope_enabled(eps->opts, SLOPE_EPS_INSIDE_PATH))
    {
      /* TODO report error */
      return;
    }

  fprintf(eps->file, "%.1lf %.1lf moveto\n", x, y);
}

void slope_epspen_line_to(slope_pen_t *self, slope_float_t x, slope_float_t y)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (!slope_enabled(eps->opts, SLOPE_EPS_INSIDE_PATH))
    {
      /* TODO report error */
      return;
    }

  fprintf(eps->file, "%.1lf %.1lf lineto\n", x, y);
}

void slope_epspen_close_path(slope_pen_t *self)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (!slope_enabled(eps->opts, SLOPE_EPS_INSIDE_PATH))
    {
      /* TODO report error */
      return;
    }

  fprintf(eps->file, "closepath\n");
}

void slope_epspen_set_colors(slope_pen_t * self,
                             slope_color_t stroke,
                             slope_color_t fill)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);
  eps->stroke_color   = stroke;
  eps->fill_color     = fill;
}

void slope_epspen_circle(slope_pen_t * self,
                         slope_float_t x,
                         slope_float_t y,
                         slope_float_t r)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (slope_enabled(eps->opts, SLOPE_EPS_OP_IN_PROGRESS))
    {
      /* TODO report error */
      return;
    }

  if (slope_color_is_visible(eps->stroke_color) &&
      slope_color_is_visible(eps->fill_color))
    {
      fprintf(eps->file,
              "<circle cx=\"%.1lf\" cy=\"%.1lf\" r=\"%.1lf\" "
              "stroke=\"#%08x\" fill=\"#%08x\" stroke-width=\"%.1lf\" />\n",
              x,
              y,
              r,
              eps->stroke_color,
              eps->fill_color,
              eps->stroke_width);
    }
  else if (slope_color_is_visible(eps->stroke_color))
    {
      fprintf(eps->file,
              "<circle cx=\"%.1lf\" cy=\"%.1lf\" r=\"%.1lf\" "
              "stroke=\"#%08x\" fill=\"none\" stroke-width=\"%.1lf\" />\n",
              x,
              y,
              r,
              eps->stroke_color,
              eps->stroke_width);
    }
  else if (slope_color_is_visible(eps->fill_color))
    {
      fprintf(eps->file,
              "<circle cx=\"%.1lf\" cy=\"%.1lf\" r=\"%.1lf\" "
              "fill=\"#%08x\" stroke=\"none\" />\n",
              x,
              y,
              r,
              eps->fill_color);
    }
}

void slope_epspen_set_width(slope_pen_t *self, slope_float_t width)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);
  eps->stroke_width   = width;
}

void slope_epspen_text(slope_pen_t * self,
                       slope_float_t x,
                       slope_float_t y,
                       const char *  text)
{
  slope_epspen_t *eps = SLOPE_EPSPEN(self);

  if (slope_enabled(eps->opts, SLOPE_EPS_OP_IN_PROGRESS))
    {
      /* TODO report error */
      return;
    }

  if (slope_color_is_visible(eps->stroke_color) &&
      slope_color_is_visible(eps->fill_color))
    {
      fprintf(eps->file,
              "newpath"
              "/Times-Roman findfont\n"
              "12 scalefont setfont\n"
              "%.1lf %.1lf moveto (%s) show",
              x,
              y,
              text);
    }
  else if (slope_color_is_visible(eps->stroke_color))
    {
      fprintf(eps->file,
              "newpath"
              "/Times-Roman findfont\n"
              "12 scalefont setfont\n"
              "%.1lf %.1lf moveto (%s) show",
              x,
              y,
              text);
    }
  else if (slope_color_is_visible(eps->fill_color))
    {
      fprintf(eps->file,
              "newpath"
              "/Times-Roman findfont\n"
              "12 scalefont setfont\n"
              "%.1lf %.1lf moveto (%s) show",
              x,
              y,
              text);
    }
}

/* slope_epspen.c */
