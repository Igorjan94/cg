#pragma once

#include <bits/stdc++.h>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/primitives/contour.h>
#include <cg/operations/has_intersection/segment_segment.h>

//I know it is very bad, but it is simple to write. p -- point, s -- segment. and scalar
#define ps point_2t<Scalar>
#define ss segment_2t<Scalar>

namespace cg
{
    template<typename Scalar>
    bool checkIfSegmentIntersectsVerticalRayFromV(ps& v, ss& segment)
    {
        //orient segment that (v, a[0], a[1]) is right triple
        auto ori = orientation(v, segment[0], segment[1]);
        if (ori == CG_LEFT || (ori == CG_COLLINEAR && collinear_are_ordered_along_line(v, segment[1], segment[0])))
        {
            ps x = segment[0];
            segment[0] = segment[1];
            segment[1] = x;
        }
        
        //if segment starts or ends in v, do not take it into consideration
        if (v == segment[0] || v == segment[1])
            return false;

        //check if segment is upper than v and intersects vertical ray
        ps vdown({v.x, v.y - 1});
        auto or1 = orientation(vdown, v, segment[0]), or2 = orientation(vdown, v, segment[1]);
        return ((or1 == CG_LEFT || (or1 == CG_COLLINEAR && segment[0].y >= v.y)) && 
               (or2 == CG_RIGHT || (or2 == CG_COLLINEAR && segment[1].y >= v.y)));
    }

    /*
        input:  vector<contour>
        output: vector<segment>
        task:   segment in contour is wall, from every point get visible vertices. Segments in contours must NOT intersect, they can only begin/end in one point
    */
    template <class Scalar>
    std::vector<cg::segment_2t<Scalar>> visibilityGraph(std::vector<cg::contour_2t<Scalar>> contours)
    {
        std::vector<ss> answer;    //all segments, which does not intersect any segment of contours
        std::vector<ss> segments;  //all segments of contours
        std::set<ps> points;       //all points, sorted by x, otherwise y

        for (auto& contour : contours)
            for (int j = 0; j + 1 < (int) contour.size(); ++j)
                if (contour[j] != contour[j + 1])
                    points.insert(contour[j]),                          //all points
                    segments.emplace_back(contour[j], contour[j + 1]);  //all segments, they are unique

        for (ps v : points)        //for each point
        {
            std::vector<ps> curPoints; //all points, which have x-coordinate not less than v's
            std::map<ps, std::vector<int>> begins, ends; //for each point store segments, which begin and end in it(to fast status refresh)

            for (ps q : points)
                if (q.x >= v.x && v != q)
                    curPoints.push_back(q);

            //sort curPoints by angle
            sort(curPoints.begin(), curPoints.end(), [&v](ps const &a, ps const &b)
            {
                //a < b  if  (v, a, b) is right triple or they are on one line in this order
                auto ori = orientation(v, a, b);
                if (ori == CG_COLLINEAR)
                    return collinear_are_ordered_along_line(v, a, b);
                return ori == CG_RIGHT;
            }); 

            //comparator -- distance from v to segments
            //note the fact, that exists ray, which intersect both segments and they do not intersect each other
            auto cmp = [&v](ss const& a, ss const& b)
            {
                if (a == b)       //it is one segment
                    return false;

                if (a[0] == b[0]) //start in one point
                    return orientation(a[1], b[0], b[1]) == CG_RIGHT;

                if (a[1] == b[1]) //finish in one point
                    return orientation(b[0], b[1], a[0]) == CG_RIGHT;

                auto ori = orientation(v, a[0], b[0]);

                //if collinear, then compare a[0] and b[0]
                if (ori == CG_COLLINEAR)
                    return collinear_are_ordered_along_line(v, a[0], b[0]);

                //a < b  if  b[0] is behind a  or  b begins before a && b[1] is behind a
                return (ori == CG_RIGHT && orientation(a[1], a[0], b[0]) == CG_RIGHT) ||
                       (ori == CG_LEFT  && orientation(b[0], b[1], a[0]) == CG_RIGHT);
            };

            std::set<ss, decltype(cmp)> status(cmp);
            for (int i = 0; i < (int) segments.size(); ++i)
            {
                //initializzzzing status. Note that function with such long name orients segment, that (v, a[0], a[1]) is right triple
                if (checkIfSegmentIntersectsVerticalRayFromV(v, segments[i]))
                    status.insert(segments[i]);
                //info for deleting and inserting segments in status by point
                begins[segments[i][0]].push_back(i);
                ends[segments[i][1]].push_back(i);
            }

            for (ps p : curPoints)
            {
                if (status.size())
                {
                    //p is visible means that closest segment does not intersect (v, p) or is one of its vertices
                    ss closest = *status.begin();
                    if (!has_intersection({v, p}, closest) || closest[0] == p || closest[1] == p)
                        answer.push_back({v, p});
                }
                else //status is empty, p is also visible
                    answer.push_back({v, p});

                //updating status:
                //delete all segments, ending in this point
                for (auto i : ends[p])
                    status.erase(segments[i]);

                //add all segments, starting in this point
                for (auto i : begins[p])
                    status.insert(segments[i]);
            }
        }
        return answer;
    }

    //naive but simple solution. O(n^3)
    template <class FwdIter, class Scalar>
    bool has_intersection_list_of_contours_segment(FwdIter begin, FwdIter end, cg::segment_2t<Scalar> const& segment)
    {
        bool has_intersection = false;
        for (auto checkContour = begin; checkContour != end; checkContour++)
            for (auto a = (*checkContour).begin(), b = a + 1; b != (*checkContour).end(); a++, b++)
                if (segment[0] != *a && segment[0] != *b && segment[1] != *a && segment[1] != *b)
                    has_intersection |= cg::has_intersection(segment, {*a, *b});
        return has_intersection;
    }

    template <class FwdIter, class Scalar>
    std::vector<cg::segment_2t<Scalar>> visibilityGraphO_N_3(FwdIter begin, FwdIter end, Scalar s)
    {
        std::vector<cg::segment_2t<Scalar>> vec;
        if (begin == end)
            return std::move(vec);
        for (auto iter = begin; iter != end; iter++)                                        //contour
            for (auto pointIter = (*iter).begin(); pointIter != (*iter).end(); pointIter++) //point
                for (auto contourIter = begin; contourIter != end; contourIter++)           //other contour
                    if (contourIter != iter)                                                //if contour != contour with point
                        for (auto otherPoint = (*contourIter).begin();
                                otherPoint != (*contourIter).end(); otherPoint++)//other point
                            if (!has_intersection_list_of_contours_segment(begin, end, cg::segment_2t<Scalar>(*pointIter, *otherPoint)))
                                vec.push_back({*pointIter, *otherPoint});
        return std::move(vec);
    }

} //namespace cg;
