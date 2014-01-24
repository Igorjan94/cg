#pragma once

#include <cg/primitives/point.h>
#include <cg/primitives/contour.h>
#include <cg/operations/contains/contour_point.h>
#include <cg/convex_hull/graham.h>
#include <cg/operations/intersection/segment_segment.h>

namespace cg
{
    template<class Scalar>
    inline cg::contour_2t<Scalar> intersection(cg::contour_2t<Scalar> & a, cg::contour_2t<Scalar> & b)
    {
        cg::contour_2t<Scalar> d;
        std::vector<cg::point_2t<Scalar>> c;
        for (int i = 0; i < a.size() - 1; i++)
            for (int j = 0; j < b.size() - 1; j++)
            {
                auto it = cg::intersection(cg::segment_2t<Scalar>(a[i], a[i + 1]), cg::segment_2t<Scalar>(b[j], b[j + 1]));
                if (it.which() == 1) //if intersection of segments is point
                    c.push_back(boost::get<cg::point_2t<Scalar>>(it));
            }
        for (auto temp : b)
            if (cg::contains(a, temp))
                c.push_back(temp);
        for (auto temp : a)
            if (cg::contains(b, temp))
                c.push_back(temp);
        for (auto it = c.begin(); it != graham_hull(c.begin(), c.end()); ++it)
            d.add_point(*it);
        d.add_point(*c.begin());
        return d;
    }
}
