#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include "cg/io/point.h"

#include <cg/primitives/rectangle.h>
#include <cg/primitives/point.h>

#include <cg/operations/has_intersection/rectangle_segment.h>

using cg::point_2f;
using cg::point_2;


struct triangle_contains_point_viewer : cg::visualization::viewer_adapter
{
   triangle_contains_point_viewer()
      : segment(point_2(-10, 50), point_2(100, 100)),
        rectangle(cg::range_d(0, 100), cg::range_d(0, 100))
   {}

   void draw(cg::visualization::drawer_type& drawer) const
   {
      drawer.set_color(Qt::white);

      if (cg::has_intersection(rectangle, segment))
      {
         drawer.set_color(Qt::green);
      }

      drawer.draw_line(segment[0], segment[1]);
      point_2 points[] = {rectangle.corner(0, 0), rectangle.corner(0, 1),
                          rectangle.corner(1, 1), rectangle.corner(1, 0)};

      for (size_t l = 0, lp = 3; l != 4; lp = l++)
      {
         drawer.draw_line(points[l], points[lp]);
      }
   }

   void print(cg::visualization::printer_type& p) const
   {
      p.corner_stream() << "press mouse rbutton near segment or rectangle vertex to move it"
                        << cg::visualization::endl
                        << "if segment and rectangle are green, they have intersection"
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

      for (size_t l = 0; l != 4; ++l)
      {
         const point_2& t = rectangle.corner(l / 2, l % 2);
         if (fabs(p.x - t.x) < 4 && fabs(p.y - t.y) < 4)
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
         return true;

      if (is_segment)
         segment[*idx_] = p;
      else
      {
         if (*idx_ / 2)
            rectangle.x.sup = std::max(rectangle.x.inf, (double) p.x);
         else
            rectangle.x.inf = std::min(rectangle.x.sup, (double) p.x);

         if (*idx_ % 2)
            rectangle.y.sup = std::max(rectangle.y.inf, (double) p.y);
         else
            rectangle.y.inf = std::min(rectangle.y.sup, (double) p.y);
      }

      return true;
   }

private:
   cg::segment_2 segment;
   cg::rectangle_2 rectangle;

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
