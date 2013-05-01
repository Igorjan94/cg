#pragma once

#include <cg/primitives/rectangle.h>
#include <cg/primitives/point.h>
#include <cg/operations/has_intersection/triangle_segment.h>

namespace cg
{
   inline bool contains(rectangle_2 const& rectangle, point_2 const& p)
   {
      return rectangle.contains(p);
   }

}
