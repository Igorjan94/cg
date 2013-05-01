#pragma once

#include <cg/primitives/contour.h>
#include "orientation.h"

namespace cg
{
   // c is ccw contour
   inline bool convex(contour_2 const & c);
}
