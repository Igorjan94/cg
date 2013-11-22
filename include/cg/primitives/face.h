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
        face_2t(){}

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

        face_2t<Scalar> *twin(int index)
        {
            return twins[index];
        }

        void addTwins(face_2t<Scalar> *t1, face_2t<Scalar> *t2, face_2t<Scalar> *t3)
        {
            twins[0] = t1;
            twins[1] = t2;
            twins[2] = t3;
        }
    };

    template<class Scalar>
    bool inCircle(point_2t<Scalar> const &a, point_2t<Scalar> const &b, point_2t<Scalar> const &c,
                  point_2t<Scalar> const &d)
    {
        double a00 = (a.x - d.x);
        double a01 = (a.y - d.y);
        double a02 = (a.x * a.x - d.x * d.x) + (a.y * a.y - d.y * d.y);
        double a10 = (b.x - d.x);
        double a11 = (b.y - d.y);
        double a12 = (b.x * b.x - d.x * d.x) + (b.y * b.y - d.y * d.y);
        double a20 = (c.x - d.x);
        double a21 = (c.y - d.y);
        double a22 = (c.x * c.x - d.x * d.x) + (c.y * c.y - d.y * d.y);
        double det =  a00 * a11 * a22 + a01 * a12 * a20 + a02 * a10 * a21 -
                     (a20 * a11 * a02 + a21 * a12 * a00 + a01 * a10 * a22);
        return det > 0;
    }

    template<class Scalar>
    bool faceContains(face_2t<Scalar>& f, point_2t<Scalar>& p)
    {
        if (f.isInf)
            return cg::orientation(f[0], f[1], p) == cg::CG_RIGHT;
        return cg::contains(f.triangle(), p);
    }

    template<class Scalar>
    void flip(face_2t<Scalar> &f, face_2t<Scalar> &g)
    {

    }
}
