#pragma once

#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/contour.h>
#include <cg/operations/contains/triangle_point.h>
#include <cg/convex_hull/graham.h>
#include <cg/operations/intersection/segment_segment.h>

namespace cg
{
    template<class Scalar>
    inline cg::contour_2t<Scalar> intersection(cg::triangle_2t<Scalar> & a, cg::triangle_2t<Scalar> & b)
    {
        cg::contour_2t<Scalar> d;
        std::vector<cg::point_2t<Scalar>> c;
        for (int i = 0; i < 3; i++)
        {
            if (cg::contains(a, b[i]))
                c.push_back(b[i]);
            if (cg::contains(b, a[i]))
                c.push_back(a[i]);
            for (int j = 0; j < 3; j++)
            {
                auto it = cg::intersection(a.side(i), b.side(j));
                if (it.which() == 1)
                    c.push_back(boost::get<cg::point_2t<Scalar>>(it));
            }
        }
        for (auto it = c.begin(); it != graham_hull(c.begin(), c.end()); ++it)
            d.add_point(*it);
        d.add_point(*c.begin());
        return d;
    }
}
