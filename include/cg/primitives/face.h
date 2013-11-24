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
        bool isInf = false;
        face_2t(){}

        face_2t(point_2t<Scalar> a, point_2t<Scalar> b, point_2t<Scalar> c)
            :t(a, b, c)
        {
            isInf = false;
        }

        face_2t(point_2t<Scalar> a, point_2t<Scalar> b)
        {
            isInf = true;
            point_2t<Scalar> p(0.0, 0.0);
            t = triangle_2t<Scalar>(a, b, p);
        }

        triangle_2t<Scalar> &triangle()
        {
            return t;
        }

        triangle_2t<Scalar> const &triangle() const
        {
            return t;
        }

        point_2t<Scalar>& operator[](int index)
        {
            return t[index % 3];
        }

        point_2t<Scalar>const& operator[](int index)const
        {
            return t[index % 3];
        }

        segment_2t<Scalar> side(int index)
        {
            return t.side(index % 3);
        }

        face_2t<Scalar> *twin(int index)
        {
            return twins[index % 3];
        }

        void setTwin(int index, face_2t<Scalar>& f)
        {
            twins[index % 3] = &f;
        }

        void setTwin(int index, face_2t<Scalar>* f)
        {
            twins[index % 3] = f;
        }

        void addTwins(face_2t<Scalar> *t1, face_2t<Scalar> *t2, face_2t<Scalar> *t3)
        {
            twins[0] = t1;
            twins[1] = t2;
            twins[2] = t3;
        }

        void notInf()
        {
            isInf = false;
        }

        void replaceTwin(face_2t<Scalar> from, face_2t<Scalar> *to)
        {
            for (int i = 0; i < 3; i++)
                if (*twins[i] == from)
                {
                    twins[i] = to;
                    return;
                }
        }

        bool operator==(face_2t<Scalar> const &a) const
        {
            if (isInf ^ a.isInf)
                return false; else
                return t == a.triangle();
        }

        void writeln2()
        {
            std::cout << "face : ";
            for (int i = 0; i < 3; i++)
                std::cout << t[i].x << " " << t[i].y << ((i == 2) ? "\n" : ", ");
            std::cout << "twins: ";
            for (int j = 0; j < 3; j++)
                twins[j]->writeln();
            std::cout << "\n";
        }

        void writeln()
        {
            if (isInf)
                std::cout << "INF ";
            for (int i = 0; i < 3; i++)
                std::cout << t[i].x << " " << t[i].y << ((i == 2) ? "\n" : ", ");
        }
    };

    template<class Scalar>
    bool inCircle(point_2t<Scalar> const &a, point_2t<Scalar> const &b, point_2t<Scalar> const &c,
                  point_2t<Scalar> const &d, bool isInf)
    {
        if (isInf)
        {
            point_2t<Scalar> temp = b == point_2t<Scalar>({0.0, 0.0}) ? c : b;
            std::cout << "all is ok\n";
            std::cout << d.x << " " << d.y << "\n";
            std::cout << temp.x << " " << temp.y << "\n";
            std::cout << a.x << " " << a.y << "\n";
            std::cout << "all is ok>\n";
            return orientation(d, temp, a) == CG_RIGHT;
        }
        Scalar a00 = (a.x - d.x);
        Scalar a01 = (a.y - d.y);
        Scalar a02 = (a.x * a.x - d.x * d.x) + (a.y * a.y - d.y * d.y);
        Scalar a10 = (b.x - d.x);
        Scalar a11 = (b.y - d.y);
        Scalar a12 = (b.x * b.x - d.x * d.x) + (b.y * b.y - d.y * d.y);
        Scalar a20 = (c.x - d.x);
        Scalar a21 = (c.y - d.y);
        Scalar a22 = (c.x * c.x - d.x * d.x) + (c.y * c.y - d.y * d.y);
        Scalar det =  a00 * a11 * a22 + a01 * a12 * a20 + a02 * a10 * a21 -
                     (a20 * a11 * a02 + a21 * a12 * a00 + a01 * a10 * a22);
        return det < 0;
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
        if (!(g.isInf ^ f.isInf))
        {
            f.writeln();
            g.writeln();
            std::cout << "infinity: " << ((int)(g.isInf)) << "\n";
            int i2 = -1;
            for (int i = 0; i < 3; i++)
            {
                bool ok = false;
                for (int j = 0; j < 3; j++)
                    ok |= g[i] == f[j];
                if (!ok)
                {
                    i2 = i;
                    break;
                }
            }
            int i1 = -1;
            for (int i = 0; i < 3; i++)
            {
                bool ok = false;
                for (int j = 0; j < 3; j++)
                    ok |= g[j] == f[i];
                if (!ok)
                {
                    i1 = i;
                    break;
                }
            }
            if (i1 != -1 && i2 != -1 && inCircle(f[i1], f[i1 + 1], f[i1 + 2], g[i2], g.isInf))
            {
                std::cout << "f::i1= " << f[i1].x << " " << f[i1].y << "\n";
                std::cout << "g::i2= " << g[i2].x << " " << g[i2].y << "\n";
                std::cout << "flipping...\n";
                g[i2 + 2] = f[i1];
                f[i1 + 2] = g[i2];
                if (g.isInf)
                    g.notInf();
                g.twin(i2 + 2)->replaceTwin(g, &f);
                f.twin(i1 + 2)->replaceTwin(f, &g);

                g.setTwin(i2 + 1, f.twin(i1 + 2));
                f.setTwin(i1 + 1, g.twin(i2 + 2));
                g.setTwin(i2 + 2, f);
                f.setTwin(i1 + 2, g);

                flip(*(f.twin(i1)), f);
                flip(f, *(f.twin(i1 + 1)));
                flip(g, *(g.twin(i2)));
                flip(g, *(g.twin(i2 + 1)));
            }
        }
    }
}
/*-150, -150
 *-5, 170
 *200, 130
 *120, 10
 *
 */
