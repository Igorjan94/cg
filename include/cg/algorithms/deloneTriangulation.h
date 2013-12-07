#pragma once

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/face.h>

using cg::triangle_2t;
using cg::point_2t;

using std::vector;

namespace cg
{
    template<class Scalar>
    vector<face_2t<Scalar>> deloneTriangulation(vector<point_2t<Scalar>> &points)
    {
        vector<face_2t<Scalar>*> faces;
        vector<face_2t<Scalar>> to;
        int sz = points.size();
        if (sz < 3)
            return to;

        int i = 2;
        while ((i < sz) && (cg::orientation(points[0], points[1], points[i]) == cg::CG_COLLINEAR))
            i++;
        if (i == sz)
        {
            for (int j = 0; j < sz - 2; j++)
                to.push_back(face_2t<Scalar>(points[j], points[j + 1], points[j + 2]));
            return to;
        }
        if (i != 2)
            std::swap(points[i], points[2]);

        if (cg::orientation(points[0], points[1], points[2]) == cg::CG_LEFT)
        {
            printf("orientation changed\n");
            std::swap(points[1], points[2]);
        }

        faces.push_back(new face_2t<Scalar>(points[0], points[1], points[2]));
        faces.push_back(new face_2t<Scalar>(points[1], points[0]));
        faces.push_back(new face_2t<Scalar>(points[2], points[1]));
        faces.push_back(new face_2t<Scalar>(points[0], points[2]));
        faces[0]->addTwins(faces[1], faces[2], faces[3]);
        faces[1]->addTwins(faces[0], faces[3], faces[2]);
        faces[2]->addTwins(faces[0], faces[1], faces[3]);
        faces[3]->addTwins(faces[0], faces[2], faces[1]);

        for (int i = 3; i < sz; i++)
        {
            bool was = false;
            for (int j = 0; j < i && !was; j++)
                if (points[i] == points[j])
                    was = true;
            if (was)
                continue;

            point_2t<Scalar> p = points[i];
            auto it = faces.begin();
            for (; it != faces.end() && !faceContains(**it, p); it++);
            face_2t<Scalar> t = **it;
            face_2t<Scalar> *t1, *t2, *t3;
            t1 = new face_2t<Scalar>(t[0], t[1], p);
            if (t.isInf)
            {
                t2 = new face_2t<Scalar>(p, t[1]);
                t3 = new face_2t<Scalar>(t[0], p);
            } else
            {
                t2 = new face_2t<Scalar>(p, t[1], t[2]);
                t3 = new face_2t<Scalar>(t[0], p, t[2]);
            }
            t2->addTwins(t1, t.twin(1), t3);
            t3->addTwins(t1, t2, t.twin(2));
            t1->addTwins(t.twin(0), t2, t3);

            t.twin(0)->replaceTwin(t, t1);
            t.twin(1)->replaceTwin(t, t2);
            t.twin(2)->replaceTwin(t, t3);

            *it = t1;
            faces.push_back(t2);
            faces.push_back(t3);

            flip(*t1, *(t1->twin(0)));
            flip(*(t2->twin(1)), *t2);
            flip(*t3, *(t3->twin(2)));
        }
        for (auto it : faces)
            if (!it->isInf)
                to.push_back(*it);
        if (!test(to))
            std::cout << "epic fail------------------------------------------------\n";
        faces.clear();
        return to;
    }

    template<class Scalar>
    bool test(vector<cg::face_2t<Scalar>> faces)
    {
        bool all = true;
        for (auto i : faces)
        {
            bool ok = true;
            for (auto j : faces)
                if (!(i == j) && !i.isInf && cg::orientation(i[0], i[1], i[2]) != cg::CG_COLLINEAR)
                    for (int k = 0; k < 3 - (int)j.isInf; k++)
                        ok &= !cg::inCircle(i[0], i[1], i[2], j[k], false);
            all &= ok;
        }
        return all;
    }
} //namespace cg;
