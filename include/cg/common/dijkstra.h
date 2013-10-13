#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <queue>

template <class Scalar>
std::vector<int> dijkstra(std::vector<std::vector<std::pair<int, Scalar>>> &edges, int n)
{
    const Scalar INF = 10000000000000007;
    std::vector<Scalar> d;
    std::priority_queue< std::pair<Scalar, int> > q;
    int s = 0, f = n, u, v, curd, w;
    d.resize(n + 1, INF);
    q.push(std::make_pair(0, s));
    d[s] = 0;
    std::vector<int> ans, p;
    p.resize(n + 1);
    if (s == f)
        return std::move(ans);
    while (!q.empty())
    {
          u = q.top().second;
          curd = -q.top().first;
          q.pop();
          if (curd > d[u])
             continue;
          for (int i = 0; i < edges[u].size(); i++)
          {
              v = edges[u][i].first;
              w = edges[u][i].second;
              if (u == v)
                  continue;
              if (d[v] > d[u] + w)
              {
                  d[v] = d[u] + w;
                  q.push({-d[v], v});
                  p[v] = u;
              }
          }
    }
    for (int v = f; v != s; v = p[v])
        ans.push_back(v);

    ans.push_back(s);
    reverse(ans.begin(), ans.end());
    return std::move(ans);
}
