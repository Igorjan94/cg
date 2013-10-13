#pragma once

#include <algorithm>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/primitives/contour.h>
#include <cg/operations/has_intersection/segment_segment.h>

namespace cg
{
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
                        {
                            bool has_intersection = false;
                            cg::segment_2t<Scalar> segment(*pointIter, *otherPoint);
                            for (auto checkContour = begin; checkContour != end; checkContour++)
                                for (auto a = (*checkContour).begin(), b = a + 1; b != (*checkContour).end(); a++, b++)
                                    if (segment[0] != *a && segment[0] != *b &&
                                        segment[1] != *a && segment[1] != *b)
                                    has_intersection |= cg::has_intersection(segment, {*a, *b});
                            if (!has_intersection)
                                vec.push_back(segment);
                        }
        return std::move(vec);
    }

} //namespace cg;
