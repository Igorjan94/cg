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
    void addPoint(int& index, point_2t<Scalar> p, vector<face_2t<Scalar>> &points)
    {
        face_2t<Scalar> t = points[index];
        if (t.isInf)
        {
            face_2t<Scalar> t1(t[0], t[1], p),
                            t2(p, t[1]),
                            t3(t[0], p);
            points[index] = t1;
            points.push_back(t2);
            points.push_back(t3);
        } else
        {
            face_2t<Scalar> t1(t[0], t[1], p),
                            t2(t[1], t[2], p),
                            t3(t[2], t[0], p);
            points[index] = t1;
            points.push_back(t2);
            points.push_back(t3);
        }
//            t1.addTwins(t.twin(0), t2, t3);
  //          t2.addTwins(t.twin(1), t3, t1);
    //        t3.addTwins(t.twin(2), t1, t2);
    }

    template<class Scalar>
    void addPointInTriangulation(point_2t<Scalar> &p, vector<face_2t<Scalar>> &points)
    {
        int i = 0;
        while (i < points.size() && !faceContains(points[i], p))
            i++;
        addPoint(i, p, points);
    }

    template<class Scalar>
    vector<face_2t<Scalar>> deloneTriangulation(vector<point_2t<Scalar>> &points)
    {
        vector<face_2t<Scalar>> ans;
        int sz = points.size();
        if (sz < 3)
            return ans;
        if (cg::orientation(points[0], points[1], points[2]) == cg::CG_LEFT)
        {
            printf("orientation changed\n");
            point_2t<Scalar> p = points[1];
            points[1] = points[2];
            points[2] = p;
        }
        ans.push_back(face_2t<Scalar>(points[0], points[1], points[2]));
        ans.push_back(face_2t<Scalar>(points[1], points[0]));
        ans.push_back(face_2t<Scalar>(points[2], points[1]));
        ans.push_back(face_2t<Scalar>(points[0], points[2]));
        for (int i = 3; i < sz; i++)
            addPointInTriangulation(points[i], ans);
        return std::move(ans);
    }
} //namespace cg;
