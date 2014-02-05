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
    Scalar projection(point_2t<Scalar> p, segment_2t<Scalar> seg)
    {
        auto v = p - seg[0];
        auto s = seg[1] - seg[0];
        return (v * s) / (s * s);
    }

    template<class Scalar>
    std::pair<cg::point_2t<Scalar>, cg::point_2t<Scalar>> distanceTwoPoligons(contour_2t<Scalar>& a, contour_2t<Scalar>& b)
    {
        if (cg::orientation(a[0], a[1], a[2]) == CG_RIGHT)
            reverse(a);
        if (cg::orientation(b[0], b[1], b[2]) == CG_RIGHT)
            reverse(b);
        auto p = std::min_element(a.begin(), a.end());
        auto q = std::max_element(b.begin(), b.end());
        auto next = [] (cg::contour_2t<Scalar> const& a, __gnu_cxx::__normal_iterator<const cg::point_2t<Scalar>*, std::vector<cg::point_2t<Scalar>, std::allocator<cg::point_2t<Scalar>>>> p)
        {
            ++p;
            if (p == a.end())
                p = a.begin();
            return p;
        };
        auto less = [] (cg::point_2t<Scalar> const &a, cg::point_2t<Scalar> const &b,
                        cg::point_2t<Scalar> const &c, cg::point_2t<Scalar> const &d)
        {
            return orientation(a, b, c, d) != CG_RIGHT;
        };

        auto min = p;
        auto max = q;
        auto p_n = p;
        auto q_n = q;
        p_n = next(a, p_n);
        q_n = next(b, q_n);
        std::pair<point_2t<Scalar>, point_2t<Scalar>> ans = std::make_pair(*p, *q);
        do
        {
            if (compare_dist(*p, *q, ans.first, ans.second))
                ans = std::make_pair(*p, *q);
            bool less1 = less(*p, *p_n, *q_n, *q);
            bool less2 = less(*p, *p_n, *q, *q_n);
            if (less1)
            {
                segment_2t<Scalar> seg(*p, *p_n);
                auto pr = projection(*q, seg);

                if (pr >= 0 && pr <= 1)
                {
                    auto proj = seg[0] + pr * (seg[1] - seg[0]);
                    if (compare_dist(proj, *q, ans.first, ans.second))
                        ans = std::make_pair(proj, *q);
                }
                p = p_n;
                p_n = next(a, p_n);
                if (compare_dist(*p, *q, ans.first, ans.second))
                    ans = std::make_pair(*p, *q);
            }
            if (less2)
            {
                segment_2t<Scalar> seg(*q, *q_n);
                auto pr = projection(*p, seg);
                if (pr >= 0 && pr <= 1)
                {
                    auto proj = seg[0] + pr * (seg[1] - seg[0]);
                    if (compare_dist(proj, *p, ans.first, ans.second))
                        ans = std::make_pair(proj, *p);
                }
                q = q_n;
                q_n = next(b, q_n);

                if (compare_dist(*p, *q, ans.first, ans.second))
                    ans = std::make_pair(*p, *q);
            }
      }
      while (p != min || q != max);
      return ans;
   }
} //namespace cg;
