#pragma once

#include <QtOpenGL>

#include "cg/visualization/viewer.h"

namespace cg {
namespace visualization
{
   struct drawer_impl : drawer_type
   {
      void set_color(QColor const & c);
      void draw_line(segment_2f const &, float width);
      void draw_line(point_2f const &, point_2f const &, float width);
      void draw_point(point_2f const & pt, float radius);

      drawer_impl()
         : current_color_ (Qt::black)
      {}

      void clear();

      struct point_buffer_t
      {
         std::vector<GLfloat>    points;
         std::vector<GLfloat>    colors;
         float radius;
      };

      struct segment_buffer_t
      {
         std::vector<GLfloat>    segments;
         std::vector<GLfloat>    colors;
         float width;
      };

      std::vector<point_buffer_t>     point_buffers;
      std::vector<segment_buffer_t>   segment_buffers;

   private:
      QColor current_color_;
   };
}}
