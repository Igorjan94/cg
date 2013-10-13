#pragma once

#include <algorithm>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/common/dijkstra.h>
#include <cg/algorithms/visibilityGraph.h>

namespace cg
{
    template <class FwdIter, class Scalar>
    std::vector<cg::point_2t<Scalar>> shortestPath(FwdIter begin, FwdIter end, cg::point_2t<Scalar> &start, cg::point_2t<Scalar> &finish)
    {
        std::vector<cg::point_2t<Scalar>> ans;
        if (start == finish)
            return {start};
        if (begin == end || !cg::has_intersection_list_of_contours_segment(begin, end, segment_2t<Scalar>(start, finish)))
        {
            ans.push_back(start);
            ans.push_back(finish);
            return std::move(ans);
        }
        std::vector<cg::point_2t<Scalar>> points;
        points.push_back(start);
        int n;
        for (auto it = begin; it != end; it++)
            for (auto p = (*it).begin(); p != (*it).end(); p++)
                points.push_back(*p);
        n = points.size();
        points.push_back(finish);
        std::vector<std::pair<int, int>> visibilityGraphNumbers = cg::visibilityGraphNumbers(begin, end, start.x);
        std::vector<std::vector<std::pair<int, Scalar>>> edges;
        edges.resize(n + 2);
        int counter = 1;
        for (auto it = begin; it != end; it++)
            for (auto p = (*it).begin(); p != (*it).end(); p++, counter++)
            {
                if (p + 1 != (*it).end())
                    edges[counter].push_back({counter + 1, cg::dist(points[counter + 1], points[counter])}),
                    edges[counter + 1].push_back({counter, cg::dist(points[counter + 1], points[counter])});
                if (!cg::has_intersection_list_of_contours_segment(begin, end, segment_2t<Scalar>(start, *p)))
                    edges[0].push_back({counter, cg::dist(points[0], points[counter])}),
                    edges[counter].push_back({0, cg::dist(points[0], points[counter])});
                if (!cg::has_intersection_list_of_contours_segment(begin, end, segment_2t<Scalar>(finish, *p)))
                    edges[n].push_back({counter, cg::dist(points[n], points[counter])}),
                    edges[counter].push_back({n, cg::dist(points[n], points[counter])});
            }
        for (int i = 0; i < visibilityGraphNumbers.size(); i++)
            edges[visibilityGraphNumbers[i].first + 1].push_back({visibilityGraphNumbers[i].second + 1,
                                                                  cg::dist(points[visibilityGraphNumbers[i].first + 1],
                                                                            points[visibilityGraphNumbers[i].second + 1])});
        std::vector<int> dij = dijkstra(edges, n);
        for (int i = 0; i < dij.size(); i++)
            ans.push_back(points[dij[i]]);
        return std::move(ans);
    }

} //namespace cg;
