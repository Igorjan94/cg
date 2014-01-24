#pragma once

#include <boost/range/algorithm/sort.hpp>

#include <cg/operations/orientation.h>

namespace cg
{

   template<class Itr>
   Itr quick_hull(Itr begin, Itr end);
}
