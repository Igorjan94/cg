#pragma once

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <cg/primitives/point.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>

using cg::point_2t;
using cg::contour_2t;

using std::vector;

namespace cg
{
    template<class Scalar>
    void reverse(contour_2t<Scalar>& a)
    {
        if (cg::orientation(a[0], a[1], a[2]) == CG_RIGHT)
        {
            contour_2t<Scalar> a1;
            for (int i = a.size() - 1; i >= 0; i--)
                a1.add_point(a[i]);
            a = a1;
        }
    }

    template<class Scalar>
    contour_2t<Scalar> minkowskiSum(contour_2t<Scalar>& a, contour_2t<Scalar>& b)
    {
        reverse(a);
        reverse(b);
        contour_2t<Scalar> c;
        int i, j, k;
        for (i = k = 0; k < a.size(); k++)
            if (a[i] < a[k])
                i = k;
        for (j = k = 0; k < b.size(); k++)
            if (b[j] < b[k])
                j = k;
        c.add_point(a[i] + b[j]);
        vector<bool> useda(a.size(), false),
                     usedb(b.size(), false);
        while (true)
        {
            if (i == a.size() - 1) i = 0;
            if (j == b.size() - 1) j = 0;
            if (useda[i] && usedb[j])
                break;
            if (useda[i] || !usedb[j] && (a[i + 1].x - a[i].x) * (b[j + 1].y - b[j].y) <
                                         (a[i + 1].y - a[i].y) * (b[j + 1].x - b[j].x))
            {
                c.add_point(c[c.size() - 1] + b[j + 1] + (-b[j]));
                usedb[j++] = true;
            } else
            {
                c.add_point(c[c.size() - 1] + a[i + 1] + (-a[i]));
                useda[i++] = true;
            }
        }
        return std::move(c);
    }
} //namespace cg;
