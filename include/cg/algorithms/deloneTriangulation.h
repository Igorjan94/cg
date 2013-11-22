#pragma once

#include <algorithm>
#include <stdio.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/face.h>

using cg::triangle_2t;
using cg::point_2t;

using std::vector;

namespace cg
{
    template<class Scalar>
    void addPoint(int& index, point_2t<Scalar> p, vector<face_2t<Scalar>> &faces)
    {
        face_2t<Scalar> t = faces[index];
        face_2t<Scalar> t1, t2, t3;
        t1 = face_2t<Scalar>(t[0], t[1], p);
        if (t.isInf)
        {
            t2 = face_2t<Scalar>(p, t[1]);
            t3 = face_2t<Scalar>(t[0], p);
        } else
        {
            t2 = face_2t<Scalar>(t[1], t[2], p);
            t3 = face_2t<Scalar>(t[2], t[0], p);
        }
        faces[index] = t1;
        faces.push_back(t2);
        faces.push_back(t3);
        t1.addTwins(t.twin(0), &t2, &t3);
        t2.addTwins(t.twin(1), &t3, &t1);
        t3.addTwins(t.twin(2), &t1, &t2);


        flip(faces[index], *faces[index].twin(0));
        index = faces.size() - 2;
        flip(faces[index], *faces[index].twin(0));
        index++;
        flip(faces[index], *faces[index].twin(0));
    }

    template<class Scalar>
    void addPointInTriangulation(point_2t<Scalar> &p, vector<face_2t<Scalar>> &faces)
    {
        int i = 0;
        while (i < faces.size() && !faceContains(faces[i], p))
            i++;
        addPoint(i, p, faces);
    }

    template<class Scalar>
    vector<face_2t<Scalar>> deloneTriangulation(vector<point_2t<Scalar>> &points)
    {
        vector<face_2t<Scalar>> faces;
        int sz = points.size();
        if (sz < 3)
            return faces;
        if (cg::orientation(points[0], points[1], points[2]) == cg::CG_LEFT)
        {
            printf("orientation changed\n");
            point_2t<Scalar> p = points[1];
            points[1] = points[2];
            points[2] = p;
        }
        faces.push_back(face_2t<Scalar>(points[0], points[1], points[2]));
        faces.push_back(face_2t<Scalar>(points[1], points[0]));
        faces.push_back(face_2t<Scalar>(points[2], points[1]));
        faces.push_back(face_2t<Scalar>(points[0], points[2]));
        faces[0].addTwins(&faces[1], &faces[2], &faces[3]);
        faces[1].addTwins(&faces[0], &faces[3], &faces[2]);
        faces[2].addTwins(&faces[0], &faces[1], &faces[3]);
        faces[3].addTwins(&faces[0], &faces[2], &faces[1]);
        for (int i = 3; i < sz; i++)
            addPointInTriangulation(points[i], faces);
        return std::move(faces);
    }
} //namespace cg;
