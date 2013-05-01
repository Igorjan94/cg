#pragma once

#include <cg/primitives/contour.h>
#include <cg/primitives/point.h>
#include <cg/operations/has_intersection/segment_segment.h>

namespace cg
{

   namespace detail
   {
      enum class test_res_t
      {
         INTERSECTION, NO_INTERSECTION, ON_SEGMENT
      };

      inline test_res_t test_intersection_left_ray(point_2 const& point,
            point_2 seg_begin,
            point_2 seg_end)
      {
         if (seg_begin.x > point.x && seg_end.x > point.x)
            return test_res_t::NO_INTERSECTION;

         if (seg_begin == point || seg_end == point)
            return test_res_t::ON_SEGMENT;

         if (seg_begin.y > seg_end.y)
            std::swap(seg_begin, seg_end);

         if (seg_begin.x < point.x && seg_end.x < point.x)
         {
            return (seg_begin.y <= point.y && seg_end.y > point.y) ?
                     test_res_t::INTERSECTION : test_res_t::NO_INTERSECTION;
         }

         if (seg_end.y == seg_begin.y && seg_begin.y == point.y)
            return test_res_t::ON_SEGMENT;

         if (seg_end.y == point.y)
            return test_res_t::NO_INTERSECTION;

         segment_2 contour_segment(seg_begin, seg_end);
         segment_2 aux_segment(point, {(std::min(seg_begin, seg_end)).x, point.y});

         return (has_intersection(contour_segment, aux_segment)) ?
                  test_res_t::INTERSECTION : test_res_t::NO_INTERSECTION;
      }
   }

   // c is convex contour ccw orientation
   inline bool convex_contains(contour_2 const& contour,
                               point_2 const& point)
   {

      auto first = contour[0];

      if (orientation(first, contour[1], point) == CG_RIGHT)
         return false;

      auto itr = std::lower_bound(contour.begin() + 2, contour.end(), point,
                                  [&] (point_2 const& l, point_2 const& val)
      {
         auto o = orientation(first, l, val);
         return o == CG_LEFT;
      });

      if (itr == contour.end())
         return false;

      return orientation(*(itr - 1), *itr, point) != CG_RIGHT;
   }

   // c is ordinary contour
   inline bool contains(contour_2 const& contour, point_2 const& val)
   {
      auto circulator = contour.circulator();
      int intersection_count = 0;

      for (size_t i = 0; i < contour.vertices_num(); ++i)
      {
         auto const& p1 = *circulator;
         ++circulator;
         auto const& p2 = *circulator;
         using namespace detail;

         switch (test_intersection_left_ray(val, p1, p2))
         {
         case test_res_t::INTERSECTION:
            intersection_count++;
            break;

         case test_res_t::ON_SEGMENT:
            return true;

         // Disabling warning
         default:
               break;
         }
      }

      return intersection_count % 2 == 1;
   }
}
