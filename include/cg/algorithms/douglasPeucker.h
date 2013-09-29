#pragma once

#include <algorithm>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>

namespace cg
{

    template<class Scalar>
    Scalar dist(cg::point_2t<Scalar> const & a, cg::segment_2t<Scalar> const & s)
    {
        Scalar R1 =  cg::dist_2(a,    s[0]),
               R2 =  cg::dist_2(a,    s[1]),
               R12 = cg::dist_2(s[0], s[1]);
        if (R1 >= R2 + R12)
            return R2; else
            if (R2 >= R1 + R12)
                return R1; else
                return pow((a.x - s[0].x) * (a.y - s[1].y) - (a.x - s[1].x) * (a.y - s[0].y), 2) / R12;
    }

    template <class fwdIter, class Scalar, class OutIter>
    void douglasPeuckerImpl(fwdIter first, fwdIter last, Scalar eps, OutIter it)
    {
        if (first == last)
            return;
        cg::segment_2t<Scalar> segment(*first, *last);
        fwdIter index = std::max_element(first + 1, last, [&first, &last, &segment](cg::point_2t<Scalar> a, cg::point_2t<Scalar> b) -> bool
        {
            return dist(a, segment) <= dist(b, segment);
        });
        if (dist(*index, segment) < eps)
            return; else
            {
                douglasPeuckerImpl(first, index, eps, it);
                *it++ = *index;
                douglasPeuckerImpl(index, last, eps, it);
            }
    }

    template<class fwdIter, class Scalar, class OutIter>
    void douglasPeucker(fwdIter first, fwdIter last, Scalar eps, OutIter it)
    {
        eps = eps * eps;
        *it++ = *first;
        douglasPeuckerImpl(first, --last, eps, it);
        *it++ = *last;
    }

} //namespace cg;
