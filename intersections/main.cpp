#include <iostream>
#include <vector>

#include <cg/operations/orientation.h>
#include <cg/primitives/point.h>
#include <cg/convex_hull/andrew.h>

using namespace std;
using namespace cg;

int main()
{
    vector<point_2> v;
    for (int i = 0; i < 10; i++)
        v.push_back(point_2(i + rand() % 10, i - rand() % 10));
    auto it = v.begin();
    for (; it != v.end(); )
        cout << (*it).x << " " << (*it++).y << "\n";
    it = v.begin();
    cout << "\n\n";
    for (; it != graham_hull(v.begin(), v.end()); it++)
        cout << (*it).x << " " << (*it).y << "\n";
    return 0;
}
