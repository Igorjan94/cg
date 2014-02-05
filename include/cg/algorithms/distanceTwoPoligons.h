#pragma once

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <cg/primitives/point.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>
#include <cg/operations/compare_dist.h>
#include <cg/algorithms/minkowskiSum.h>

using cg::point_2t;
using cg::contour_2t;

using std::vector;

namespace cg
{
    template<class Scalar>
    Scalar projection(point_2t<Scalar> p, segment_2t<Scalar> segment)
    {
        auto v = p - segment[0];
        auto s = segment[1] - segment[0];
        return (v * s) / (s * s);
    }

    template<class Scalar>
    int next(cg::contour_2t<Scalar> const& a, int index)
    {
        return (index + 1) % (a.size() - 1);
    }

    template<class Scalar>
    void updateAnswer(contour_2t<Scalar> const& contour, point_2t<Scalar> const& point, int& index, std::pair<point_2t<Scalar>, point_2t<Scalar>>& ans)
    {
        segment_2t<Scalar> seg(contour[index], contour[index + 1]);
        Scalar pr = projection(point, seg);
        if (pr >= 0 && pr <= 1)
        {
            cg::point_2t<Scalar> proj = seg[0] + pr * (seg[1] - seg[0]);
            if (compare_dist(proj, point, ans.first, ans.second))
                ans = {proj, point};
        }
        index = next(contour, index);
        if (compare_dist(contour[index], point, ans.first, ans.second))
            ans = {contour[index], point};
    }

    template<class Scalar>
    std::pair<cg::point_2t<Scalar>, cg::point_2t<Scalar>> distanceTwoPoligons(contour_2t<Scalar>& a, contour_2t<Scalar>& b)
    {
        if (cg::orientation(a[0], a[1], a[2]) == CG_RIGHT)
            reverse(a);
        if (cg::orientation(b[0], b[1], b[2]) == CG_RIGHT)
            reverse(b);
        int i, j, minimum, maximum;
        for (int k = i = 0; k < a.size(); k++)
            if (a[i] > a[k])
                i = k;
        for (int k = j = 0; k < b.size(); k++)
            if (b[j] < b[k])
                j = k;
        minimum = i;
        maximum = j;

        std::pair<point_2t<Scalar>, point_2t<Scalar>> ans = std::make_pair(a[i], b[j]);
        do
        {
            if (compare_dist(a[i], b[j], ans.first, ans.second))
                ans = std::make_pair(a[i], b[j]);
            bool less1 = orientation(a[i], a[i + 1], b[j + 1], b[j]) != CG_RIGHT;
            bool less2 = orientation(a[i], a[i + 1], b[j], b[j + 1]) != CG_RIGHT;
            if (less1)
                updateAnswer(a, b[j], i, ans);
            if (less2)
                updateAnswer(b, a[i], j, ans);
        }
        while (i != minimum || j != maximum);
        return ans;
    }
} //namespace cg;
