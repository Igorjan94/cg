#pragma once

#include <iostream>

#include "cg/primitives/triangle.h"
#include "cg/primitives/point.h"
#include "cg/operations/orientation.h"
#include "cg/operations/contains/triangle_point.h"

using cg::point_2t;
using cg::triangle_2t;
using cg::segment_2t;

namespace cg
{
    template <class Scalar> struct face_2t;

    typedef face_2t<double>   face_2;
    typedef face_2t<float>    face_2f;
    typedef face_2t<int>      face_2i;

    template<class Scalar>
    struct face_2t
    {
    private:
        triangle_2t<Scalar> t;
        face_2t<Scalar>* twins[3];
    public:
        bool isInf;
        face_2t(point_2t<Scalar> a, point_2t<Scalar> b, point_2t<Scalar> c)
            :t(a, b, c)
        {
            isInf = false;
        }

        face_2t(point_2t<Scalar> a, point_2t<Scalar> b)
        {
            isInf = true;
            point_2t<Scalar> p(0, 0);
            t = triangle_2t<Scalar>(a, b, p);
        }

        triangle_2t<Scalar> &triangle()
        {
            return t;
        }

        point_2t<Scalar> operator[](int index) const
        {
            return t[index];
        }

        segment_2t<Scalar> side(int index)
        {
            return t.side(index);
        }

        void addTwins(face_2t<Scalar>& t1, face_2t<Scalar>& t2, face_2t<Scalar>& t3)
        {
            twins[0] = t1;
            twins[1] = t2;
            twins[2] = t3;
        }
    };

    template<class Scalar>
    bool faceContains(face_2t<Scalar>& f, point_2t<Scalar>& p)
    {
        if (f.isInf)
            return cg::orientation(f[0], f[1], p) == cg::CG_RIGHT;
        return cg::contains(f.triangle(), p);
    }
}
