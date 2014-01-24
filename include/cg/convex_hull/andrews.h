#pragma once

#include <boost/range/algorithm/sort.hpp>

#include <cg/primitives/point.h>
#include <cg/operations/orientation.h>
#include "graham.h"

namespace cg
{

   template<class BidItr>
   BidItr andrews(BidItr begin, BidItr end)
   {

      if (begin == end)
      {
         return begin;
      }

      std::iter_swap(begin, std::min_element(begin, end));

      BidItr second = ++begin;
      --begin;

      if (second == end)
      {
         return end;
      }

      BidItr last = --end;
      ++end;

      if (second == last)
      {
         return end;
      }

      std::iter_swap(last, std::max_element(second, end));

      BidItr upper_start = std::partition(second, last,
                                          [&begin, &last] (point_2 const& p)
      {
         return orientation(*begin, *last, p) != CG_LEFT;
      });

      std::sort(begin, upper_start);
      std::sort(upper_start, end, std::greater<point_2>());

      BidItr t = contour_graham_hull(begin, end);

      return t;
   }
}
