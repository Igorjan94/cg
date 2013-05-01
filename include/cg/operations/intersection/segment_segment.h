#pragma once

#include <boost/variant.hpp>
#include <boost/none_t.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/operations/has_intersection/segment_segment.h>

namespace cg
{
   typedef boost::variant<boost::none_t, point_2, segment_2> segment_segment_intersection_t;

   inline segment_segment_intersection_t intersection(segment_2 const & a, segment_2 const & b, int eps_pwr)
   {
      if (a[0] == a[1])
         return has_intersection(a, b) ? a[0] : boost::none;

      if (b[0] == b[1])
         return has_intersection(a, b) ? b[0] : boost::none;

      orientation_t ab[2];
      for (size_t l = 0; l != 2; ++l)
         ab[l] = orientation(a[0], a[1], b[l]);

      if (ab[0] == ab[1] && ab[0] == CG_COLLINEAR)
      {
         point_2 b = std::max(min(a), min(b));
         point_2 e = std::min(max(a), max(b));

         if (b == e) return b;
         else if (b < e) return segment_2(b, e);
         else return boost::none;
      }

      if (ab[0] == ab[1]
          || orientation(b[0], b[1], a[0]) == orientation(b[0], b[1], a[1]))
      {
         return boost::none;
      }



   }
}
