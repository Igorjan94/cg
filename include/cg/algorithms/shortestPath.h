#pragma once

#include <algorithm>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/include/cg/common/dijkstra.h>
#include <cg/include/cg/algorithms/visibilityGraph.h>

namespace cg
{
    template <class FwdIter, class OutIter, class Scalar>
    std::vector<cg::point_2t<Scalar>> visibilityGraph(FwdIter begin, FwdIter end, cg::point_2t<Scalar> start, cg::point_2t<Scalar> finish)
    {
        std::vector<cg::point_2t<Scalar>> ans;
        ans.push_back(start);
        if (start == finish)
            return std::move(ans);
        if (begin == end)
        {
            ans.push_back(finish);
            return std::move(ans);
        }
        std::vector<cg::point_2t<Scalar>> points;
        points.push_back(start);
        int counter = 0, n;
        for (auto it = begin; it != end; it++)
            for (auto p = (*it).begin(); p != (*it).end(); p++, counter++)
                points.push_back(*p);
        n = counter + 2;
        points.push_back(finish);
        std::vector<std::pair<int, int>> visibilityGraphNumbers = cg::visibilityGraphNumbers(begin, end, start.x);
        std::vector<std::vector<int>> edges;
        edges.resize(counter + 4);
        counter = 0;
        for (auto it = begin; it != end; it++)
            for (auto p = (*it).begin(); p != (*it).end(); p++, counter++)
            {
                if (cg::has_intersection_list_of_contours_segment(begin, end, {start, *p}))
                    edges[0].push_back(counter + 1),
                    edges[counter + 1].push_back(0);
                if (cg::has_intersection_list_of_contours_segment(begin, end, {end, *p}))
                    edges[n].push_back(counter + 1),
                    edges[counter + 1].push_back(n);
            }
        for (int i = 0; i < visibilityGraphNumbers.size(); i++)
            edges[visibilityGraphNumbers[i + 1].first].push_back(visibilityGraphNumbers[i].second + 1),
            edges[visibilityGraphNumbers[i + 1].second].push_back(visibilityGraphNumbers[i].first + 1);
        return std::move(dijkstra(edges, n));
    }

} //namespace cg;
