#include <gtest/gtest.h>

#include <cg/operations/orientation.h>

#include <misc/performance_profiler.h>
#include <misc/random_utils.h>
#include "random_utils.h"

using namespace util;
using namespace cg;

// -- START points orientation --
TEST(orientation, uniform_line)
{
   uniform_random_real<double, std::mt19937> distr(-(1LL << 53), (1LL << 53));

   std::vector<cg::point_2> pts = uniform_points(1000);
   util::perftest::performance_profiler pp1("external");
   for (size_t l = 0, ln = 1; ln < pts.size(); l = ln++)
   {
      cg::point_2 a = pts[l];
      cg::point_2 b = pts[ln];

      for (size_t k = 0; k != 1000; ++k)
      {
         double t = distr();
         cg::point_2 c = a + t * (b - a);
         util::perftest::performance_profiler pp2("internal", false);
         EXPECT_EQ(cg::orientation(a, b, c), *cg::orientation_r()(a, b, c));
      }
   }
}
// -- END points orientation --

// -- START contour orientation --
TEST(orientation_c, t1)
{
   contour_2 contour({{1, 1}, {2, 0}, {0, 1.1}});
   EXPECT_EQ(counterclockwise(contour), false);
}

TEST(orientation_c, t2)
{
   contour_2 contour({{1, 1}, {0, 1.1}, {2, 0}});
   EXPECT_EQ(counterclockwise(contour), true);
}

TEST(orientation_c, t3)
{
   contour_2 contour({{0, 0}, {-1, 2}, {2, 3}, {3, 4}, {1, -1}});
   EXPECT_EQ(counterclockwise(contour), false);
}

TEST(orientation_c, t4)
{
   contour_2 contour({{1, -1}, {3, 4}, {2, 3}, {-1, 2}, {0, 0}});
   EXPECT_EQ(counterclockwise(contour), true);
}
// -- END contour orientation
