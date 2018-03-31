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

#include "slope/svg_pen_p.h"

slope_pen_class_t slope_svgpen_class = {0U,
                                        slope_svgpen_destroy,
                                        slope_svgpen_begin_path,
                                        slope_svgpen_end_path,
                                        slope_svgpen_move_to,
                                        slope_svgpen_line_to,
                                        slope_svgpen_set_colors,
                                        slope_svgpen_close_path};

slope_pen_t *slope_svgpen_new(const char *   file_name,
                              slope_float_t  width,
                              slope_float_t  height,
                              slope_uint32_t opts)
{
  /*
   * Allocate memory for the pen object
   */
  slope_svgpen_t *self = slope_new(slope_svgpen_t);
  if (self == NULL) goto FAIL_alloc_pen;
  self->base.klass = &slope_svgpen_class;

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
  self->stroke_width = 1.0;
  self->opts         = opts;

  /*
   * Write file headings
   */
  slope_svgpen_initialize_file(self, width, height);

  /*
   * Return new object of handle errors
   */
  return (slope_pen_t *) self;
FAIL_open_file:
  slope_free(self);
FAIL_alloc_pen:
  return NULL;
}

void slope_svgpen_initialize_file(slope_svgpen_t *self,
                                  slope_float_t   width,
                                  slope_float_t   height)
{
  if (slope_enabled(self->opts, SLOPE_SVG_OUTPUT_HTML))
    {
      fprintf(self->file,
              "<!DOCTYPE html>\n"
              "<!-- Created with slope -->\n"
              "<html>\n"
              "<body>\n"
              "<svg\n"
              "  width=\"%.1lfpx\"\n"
              "  height=\"%.1lfpx\"\n"
              /*  viewBox=\"0 0 210 297\"\n"*/
              "  version=\"1.1\"\n"
              ">\n",
              width,
              height);
    }
  else
    {
      fprintf(self->file,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
              "<!-- Created with slope -->\n"
              "<svg\n"
              "  xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
              "  xmlns:cc=\"http://creativecommons.org/ns#\"\n"
              "  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
              "  xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
              "  xmlns=\"http://www.w3.org/2000/svg\"\n"
              "  width=\"%.1lfpx\"\n"
              "  height=\"%.1lfpx\"\n"
              /*  viewBox=\"0 0 210 297\"\n"*/
              "  version=\"1.1\"\n"
              ">\n",
              width,
              height);
    }
}

void slope_svgpen_finalize_file(slope_svgpen_t *self)
{
  if (slope_enabled(self->opts, SLOPE_SVG_OUTPUT_HTML))
    {
      fprintf(self->file,
              "</svg>\n"
              "</body>\n"
              "</html>\n");
    }
  else
    {
      fprintf(self->file, "</svg>\n");
    }
}

void slope_svgpen_destroy(slope_pen_t *self)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);

  /*
   * Write file ending tags
   */
  slope_svgpen_finalize_file(svg);

  /*
   * Close file and free file name
   */
  if (svg->file_name) slope_free(svg->file_name);
  fclose(svg->file);

  /*
   * Free pen object memory
   */
  slope_free(self);
}

void slope_svgpen_begin_path(slope_pen_t *self)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);
  fprintf(svg->file, "<path d=\"");
}

void slope_svgpen_end_path(slope_pen_t *self)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);

  if (slope_color_is_visible(svg->stroke_color) &&
      slope_color_is_visible(svg->fill_color))
    {
      fprintf(svg->file,
              "\" stroke=\"#%08x\" fill=\"#%08x\" />\n",
              svg->stroke_color,
              svg->fill_color);
    }
  else if (slope_color_is_visible(svg->stroke_color))
    {
      fprintf(svg->file,
              "\" stroke=\"#%08x\" fill=\"none\" />\n",
              svg->stroke_color);
    }
  else if (slope_color_is_visible(svg->fill_color))
    {
      fprintf(
          svg->file, "\" fill=\"#%08x\" stroke=\"none\"/>\n", svg->fill_color);
    }
}

void slope_svgpen_move_to(slope_pen_t *self, slope_float_t x, slope_float_t y)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);
  fprintf(svg->file, "M%.1lf %.1lf ", x, y);
}

void slope_svgpen_line_to(slope_pen_t *self, slope_float_t x, slope_float_t y)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);
  fprintf(svg->file, "L%.1lf %.1lf ", x, y);
}

void slope_svgpen_close_path(slope_pen_t *self)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);
  fprintf(svg->file, "Z ");
}

void slope_svgpen_set_colors(slope_pen_t * self,
                             slope_color_t stroke,
                             slope_color_t fill)
{
  slope_svgpen_t *svg = SLOPE_SVGPEN(self);
  svg->stroke_color   = stroke;
  svg->fill_color     = fill;
}

void slope_svgpen_set_html_output(slope_svgpen_t *self, slope_bool_t enable)
{
  slope_enable(self->opts, SLOPE_SVG_OUTPUT_HTML, enable);
}

/* slope_svgpen.c */
