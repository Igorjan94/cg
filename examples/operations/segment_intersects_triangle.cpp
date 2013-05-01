#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include "cg/io/point.h"

#include <cg/primitives/triangle.h>
#include <cg/primitives/point.h>

#include <cg/operations/has_intersection/triangle_segment.h>

using cg::point_2f;
using cg::point_2;


struct triangle_contains_point_viewer : cg::visualization::viewer_adapter
{
   triangle_contains_point_viewer()
      : segment(point_2(0, 50), point_2(100, 100)),
        triangle(point_2(0, 0), point_2(50, 100), point_2(100, 0))
   {}

   void draw(cg::visualization::drawer_type& drawer) const
   {
      drawer.set_color(Qt::white);

      if (cg::has_intersection(triangle, segment))
      {
         drawer.set_color(Qt::green);
      }

      drawer.draw_line(segment[0], segment[1]);
      for (size_t l = 0, lp = 2; l != 3; lp = l++)
      {
         drawer.draw_line(triangle[l], triangle[lp]);
      }
   }

   void print(cg::visualization::printer_type& p) const
   {
      p.corner_stream() << "press mouse rbutton near segment or triangle vertex to move it"
                        << cg::visualization::endl
                        << "if segment and triangle are green, they have intersection"
                        << cg::visualization::endl;
   }

   bool on_press(const point_2f& p)
   {
      for (size_t l = 0; l != 2; ++l)
      {
         if (fabs(p.x - segment[l].x) < 4 && fabs(p.y - segment[l].y) < 4)
         {
            idx_ = l;
            is_segment = true;
            return true;
         }
      }

      for (size_t l = 0; l != 3; ++l)
      {
         if (fabs(p.x - triangle[l].x) < 4 && fabs(p.y - triangle[l].y) < 4)
         {
            idx_ = l;
            is_segment = false;
            return true;
         }
      }

      return false;
   }

   bool on_release(const point_2f& p)
   {
      idx_.reset();
      return false;
   }

   bool on_move(const point_2f& p)
   {
      current_point_ = p;

      if (!idx_)
      {
         return true;
      }

      if (is_segment)
      {
         segment[*idx_] = p;
      }
      else
      {
         triangle[*idx_] = p;
      }

      return true;
   }

private:
   cg::segment_2 segment;
   cg::triangle_2 triangle;

   bool is_segment;
   boost::optional<size_t> idx_;
   boost::optional<cg::point_2> current_point_;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   triangle_contains_point_viewer viewer;
   cg::visualization::run_viewer(&viewer, "triangle contains point");
}
