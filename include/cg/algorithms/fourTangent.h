#pragma once

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <cg/primitives/point.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>
#include <cg/algorithms/minkowskiSum.h>

using cg::point_2t;
using cg::contour_2t;

using std::vector;

namespace cg
{
    template<class Scalar>
    std::vector<point_2t<Scalar>> get(point_2t<Scalar> p, contour_2t<Scalar>& b)
    {
        int j = 0;
        while (cg::orientation(b[j], b[j + 1], p) == cg::CG_RIGHT)
            j = (j + 1) % (b.size() - 1);
        int k = j;
        while (cg::orientation(b[k], b[k + 1], p) == cg::CG_LEFT)
            k = k == 0 ? b.size() - 2 : k - 1;
        while (cg::orientation(b[j], b[j + 1], p) == cg::CG_LEFT)
            j = (j + 1) % (b.size() - 1);
        return {b[(k + 1) % (b.size() - 1)], b[j]};
    }

    template<class Scalar>
    std::vector<std::pair<cg::point_2t<Scalar>, cg::point_2t<Scalar>>> fourTangent2(contour_2t<Scalar>& a, contour_2t<Scalar>& b)
    {
        if (cg::orientation(a[0], a[1], a[2]) == CG_LEFT)
            reverse(a);
        if (cg::orientation(b[0], b[1], b[2]) == CG_LEFT)
            reverse(b);
        std::vector<std::pair<cg::point_2t<Scalar>, cg::point_2t<Scalar>>> c;
        for (int i = 0; i < a.size() - 1; i++)
            for (point_2t<Scalar> x : get(a[i], b))
            {
                if (   cg::orientation(a[i == 0 ? a.size() - 2 : i - 1], a[i], x) == cg::CG_RIGHT
                    && cg::orientation(a[i], a[i + 1], x) == cg::CG_LEFT)
                    c.push_back({a[i], x});
                if (   cg::orientation(a[i == 0 ? a.size() - 2 : i - 1], a[i], x) == cg::CG_LEFT
                    && cg::orientation(a[i], a[i + 1], x) == cg::CG_RIGHT)
                    c.push_back({a[i], x});
            }
        return std::move(c);
    }

    template<class Scalar>
    int previous(cg::contour_2t<Scalar> const& a, int index)
    {
        return index == 0 ? a.size() - 2 : (index - 1);
    }

    template<class Scalar>
    int next(cg::contour_2t<Scalar> const& a, int index)
    {
        return (index + 1) % (a.size() - 1);
    }

    template<class Scalar>
    void updateAnswer(contour_2t<Scalar> const& a, contour_2t<Scalar> const& b, int& i, int& j, std::vector<std::pair<point_2t<Scalar>, point_2t<Scalar>>>& c)
    {
        if (cg::opposite(cg::orientation(a[i], a[i + 1], b[j]), cg::orientation(a[previous(a, i)], a[i], b[j]))
         && cg::opposite(cg::orientation(b[j], b[j + 1], a[i]), cg::orientation(b[previous(b, j)], b[j], a[i])))
            c.push_back({a[i], b[j]});
        i = next(a, i);
    }

    template<class Scalar>
    std::vector<std::pair<cg::point_2t<Scalar>, cg::point_2t<Scalar>>> fourTangent(contour_2t<Scalar>& a, contour_2t<Scalar>& b)
    {
        std::vector<std::pair<cg::point_2t<Scalar>, cg::point_2t<Scalar>>> c;
        if (cg::orientation(a[0], a[1], a[2]) == CG_RIGHT)
            reverse(a);
        if (cg::orientation(b[0], b[1], b[2]) == CG_RIGHT)
            reverse(b);
        int i, j, minimum, maximum;
        for (int k = i = 0; k < a.size() - 1; k++)
            if (a[i] > a[k])
                i = k;
        for (int k = j = 0; k < b.size() - 1; k++)
            if (b[j] < b[k])
                j = k;
        minimum = i;
        maximum = j;
        do
        {
            bool less1 = orientation(a[i], a[i + 1], b[j + 1], b[j]) != CG_RIGHT;
            bool less2 = orientation(a[i], a[i + 1], b[j], b[j + 1]) != CG_RIGHT;
            if (less1)
                updateAnswer(a, b, i, j, c);
            if (less2)
                updateAnswer(b, a, j, i, c);
        }
        while (i != minimum || j != maximum);

        for (int k = j = 0; k < b.size() - 1; k++)
            if (b[j] > b[k])
                j = k;
        maximum = j;
        do
        {
            bool less1 = orientation(a[i + 1], a[i], b[j + 1], b[j]) != CG_RIGHT;
            bool less2 = orientation(a[i + 1], a[i], b[j], b[j + 1]) != CG_RIGHT;
            if (less1)
                updateAnswer(a, b, i, j, c);
            if (less2)
                updateAnswer(b, a, j, i, c);
        }
        while (i != minimum || j != maximum);
        return std::move(c);
    }
} //namespace cg;
