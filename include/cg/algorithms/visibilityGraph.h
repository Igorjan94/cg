#pragma once

#include <algorithm>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/primitives/contour.h>
#include <cg/operations/has_intersection/segment_segment.h>

namespace cg
{
    template <class FwdIter, class Scalar>
    bool has_intersection_list_of_contours_segment(FwdIter begin, FwdIter end, cg::segment_2t<Scalar> const &segment)
    {
        bool has_intersection = false;
        for (auto checkContour = begin; checkContour != end; checkContour++)
            for (auto a = (*checkContour).begin(), b = a + 1; b != (*checkContour).end(); a++, b++)
                if (segment[0] != *a && segment[0] != *b && segment[1] != *a && segment[1] != *b)
                    has_intersection |= cg::has_intersection(segment, {*a, *b});
        return has_intersection;
    }

    template <class FwdIter, class Scalar>
    std::vector<cg::segment_2t<Scalar>> visibilityGraph(FwdIter begin, FwdIter end, Scalar s)
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

    template <class FwdIter, class Scalar>
    std::vector<std::pair<int, int>> visibilityGraphNumbers(FwdIter begin, FwdIter end, Scalar s)
    {
        std::vector<std::pair<int, int>> vec;
        if (begin == end)
            return std::move(vec);
        int p1 = 0, p2;
        for (auto iter = begin; iter != end; iter++)                                        //contour
            for (auto pointIter = (*iter).begin(); pointIter != (*iter).end(); pointIter++, p1++) //point
            {
                p2 = 0;
                for (auto contourIter = begin; contourIter != end; contourIter++)           //other contour
                    if (contourIter == iter)
                        p2 += (*iter).size(); else//if contour != contour with point
                        for (auto otherPoint = (*contourIter).begin();
                                  otherPoint != (*contourIter).end(); otherPoint++, p2++)//other point
                            if (!has_intersection_list_of_contours_segment(begin, end, cg::segment_2t<Scalar>(*pointIter, *otherPoint)))
                                vec.push_back({p1, p2});
            }
        return std::move(vec);
    }

} //namespace cg;
