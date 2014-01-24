#pragma once

#include <boost/variant.hpp>
#include <boost/none_t.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/operations/has_intersection/segment_segment.h>

namespace cg
{
    template<class Scalar>
    inline boost::variant<boost::none_t, point_2t<Scalar>, segment_2t<Scalar>>
                            intersection(segment_2t<Scalar> const & a, segment_2t<Scalar> const & b)
    {
        if (a[0] == a[1])
        {
            if (has_intersection(a, b))
                return a[0];
            else
                return boost::none;
        }
        if (b[0] == b[1])
        {
            if (has_intersection(a, b))
                return b[0];
            else
                return boost::none;
        }
        orientation_t ab[2];
        for (size_t l = 0; l != 2; ++l)
            ab[l] = orientation(a[0], a[1], b[l]);
        if (ab[0] == ab[1] && ab[0] == CG_COLLINEAR)
        {
            point_2t<Scalar> beg = std::max(min(a), min(b));
            point_2t<Scalar> end = std::min(max(a), max(b));

            if (beg == end)
                return beg;
            else if (beg < end)
                return segment_2t<Scalar>(beg, end);
            else
                return boost::none;
        }

        if (ab[0] == ab[1] || orientation(b[0], b[1], a[0]) == orientation(b[0], b[1], a[1]))
            return boost::none;

         Scalar det = (a[0].x - a[1].x) * (b[0].y - b[1].y) - (a[0].y - a[1].y) * (b[0].x - b[1].x),
                t1 = a[0].x * a[1].y - a[0].y * a[1].x,
                t2 = b[0].x * b[1].y - b[0].y * b[1].x,
                x = (t1 * (b[0].x - b[1].x) - t2 * (a[0].x - a[1].x)) / det,
                y = (t1 * (b[0].y - b[1].y) - t2 * (a[0].y - a[1].y)) / det;
         return cg::point_2t<Scalar>(x, y);
   }
}
