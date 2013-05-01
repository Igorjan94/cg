#pragma once

#include <boost/random.hpp>
#include <cg/primitives/point.h>
#include <misc/random_utils.h>

inline std::vector<cg::point_2> uniform_points(size_t count)
{
    util::uniform_random_real<double> rand(-10000., 10000.);

    std::vector<cg::point_2> res(count);

    for (size_t l = 0; l != count; ++l)
    {
//        rand >> res[l].x;
//        rand >> res[l].y;
       res[l].x = rand();
       res[l].y = rand();
    }

    return res;
}
