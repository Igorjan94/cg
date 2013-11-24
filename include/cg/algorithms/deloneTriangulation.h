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
    template<class Scalar, class Iter>
    void addPoint(Iter it, point_2t<Scalar> p, vector<face_2t<Scalar>*> &faces)
    {
        face_2t<Scalar> t = **it;
        face_2t<Scalar> t1, t2, t3;
        t1 = face_2t<Scalar>(t[0], t[1], p);
        if (t.isInf)
        {
            t2 = face_2t<Scalar>(p, t[1]);
            t3 = face_2t<Scalar>(t[0], p);
            t1.addTwins(t.twin(0), &t2, &t3);
            t2.addTwins(&t1, t.twin(1), &t3);
            t3.addTwins(&t1, &t2, t.twin(2));
        } else
        {
            t2 = face_2t<Scalar>(t[1], t[2], p);
            t3 = face_2t<Scalar>(t[2], t[0], p);
            t1.addTwins(t.twin(0), &t2, &t3);
            t2.addTwins(t.twin(1), &t3, &t1);
            t3.addTwins(t.twin(2), &t1, &t2);
        }
        t.twin(0)->replaceTwin(t, &t1);
        t.twin(1)->replaceTwin(t, &t2);
        t.twin(2)->replaceTwin(t, &t3);


        //faces[faces.size() - 3]->writeln2();
//        flip(t1, *(t1.twin(0)), 0);
  //      flip(t2, *(t2.twin(0)), 0);
    //    flip(t3, *(t3.twin(0)), 0);
//        tt1 = &t1;
  //      tt2 = &t2;
    //    tt3 = &t3;
        *it = &t1;
        faces.push_back(&t2);
        faces.push_back(&t3);

        /*(*it)->writeln2();
        faces[faces.size() - 2]->writeln2();
        faces[faces.size() - 1]->writeln2();*/
        for (auto it : faces)
        {
            std::cout << "debug in add: ";
            it->writeln();
        }

        for (int index = faces.size() - 2; index < faces.size(); index++)
            flip(*faces[index], *faces[index]->twin(0), 0);
    }

    template<class Scalar>
    void addPointInTriangulation(point_2t<Scalar> &p, vector<face_2t<Scalar>*> &faces)
    {

    }

    template<class Scalar>
    vector<face_2t<Scalar>> deloneTriangulation(vector<point_2t<Scalar>> &points)
    {
        vector<face_2t<Scalar>*> faces;
        vector<face_2t<Scalar>> to;
        /*points.clear();
        points.push_back({-150, -150});
        points.push_back({0, 150});
        points.push_back({150, -150});
        points.push_back({0, 0});
        points.push_back({0, -50});
*/
        int sz = points.size();
        if (sz < 3)
            return to;
        if (cg::orientation(points[0], points[1], points[2]) == cg::CG_LEFT)
        {
            printf("orientation changed\n");
            point_2t<Scalar> p = points[1];
            points[1] = points[2];
            points[2] = p;
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
                t2->addTwins(t1, t.twin(1), t3);
                t3->addTwins(t1, t2, t.twin(2));
            } else
            {
                t2 = new face_2t<Scalar>(t[1], t[2], p);
                t3 = new face_2t<Scalar>(t[2], t[0], p);
                t2->addTwins(t.twin(1), t3, t1);
                t3->addTwins(t.twin(2), t1, t2);
            }
            t1->addTwins(t.twin(0), t2, t3);
            t.twin(0)->replaceTwin(t, t1);
            t.twin(1)->replaceTwin(t, t2);
            t.twin(2)->replaceTwin(t, t3);
            flip(*t1, *(t1->twin(0)), 0);
            flip(*t2, *(t2->twin(0)), 0);
            flip(*t3, *(t3->twin(0)), 0);

            *it = t1;
            faces.push_back(t2);
            faces.push_back(t3);
        }
        for (auto it : faces)
            if (!it->isInf)
                to.push_back(*it);
        faces.clear();
        return to;
    }
} //namespace cg;
