#pragma once

#include <algorithm>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>

namespace cg
{
    template <class FwdIter, class OutIter, class Scalar>
    void visibilityGraph(FwdIter begin, FwdIter end, OutIter it, cg::point_2t<Scalar> start, cg::point_2t<Scalar> finish)
    {
        if (start == finish)
        {
            *it++ = *start;
            return;
        }
        if (begin == end)
        {
            *it++ = *start;
            *it++ = *finish;
            return;
        }

    }

} //namespace cg;
