#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>

#include <cg/algorithms/deloneTriangulation.h>
#include <cg/operations/contains/segment_point.h>

#include <cg/io/point.h>

#include "random_utils.h"

TEST(deloneTriangulation, simple)
{
   using cg::point_2;

   std::vector<point_2> pts
   {
      {0, 0},
      {1, 0},
      {0, 1},
      {2, 0},
      {0, 2},
      {3, 0}
   };

   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, simple_2)
{
   using cg::point_2;

   std::vector<point_2> pts
   {
      {0, 0},
      {1, 0},
      {0, -1},
      {2, 0},
      {0, -2},
      {3, 0}
   };

   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, s1)
{
   using cg::point_2;

   std::vector<point_2> pts
   {
      {88, 77},
      {18, 87},
      {59, 9},
      {64, 37},
      {64, 71}
   };

   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, one_line_not_axe)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {2, 2}, {1, 1}, {3, 3}, {0, 10}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, OX)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {10, 0}, {20, 0}, {30, 0}, {15, 15}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, OX2)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {30, 0}, {10, 0}, {20, 0}, {0, 0}, {15, 15}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, OY)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {0, 10}, {0, 20}, {0, 30}, {15, 15}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, OY2)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 30}, {0, 10}, {0, 20}, {0, 0}, {15, 15}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, one_line_not_infinity)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {-100, -100}, {-100, 100}, {100, 100}, {100, -100}, {0, 30}, {0, 10}, {0, 20}, {0, 0}, {15, 15}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, one_line_a_lot)
{
   using cg::point_2;

   std::vector<point_2> pts;
   for (int i = 0; i < 100; i++)
        pts.push_back({i * 10, 0});
   pts.push_back({100, 100});
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, on_circle)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 5}, {0, -5}, {5, 0}, {-5, 0}, {3, 4},  {-3, -4}, {-3, 4}, {4, -3}
   };
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, uniform)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(1000);
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}

TEST(deloneTriangulation, uniform_2)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(1000);
   EXPECT_TRUE(test(deloneTriangulation(pts)));
}
