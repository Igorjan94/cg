#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>

#include <cg/convex_hull/graham.h>
#include <cg/convex_hull/andrews.h>
#include <cg/operations/contains/segment_point.h>

#include <cg/io/point.h>

#include "random_utils.h"

template <class FwdIter>
bool is_convex_hull(FwdIter p, FwdIter c, FwdIter q)
{
   for (FwdIter t = boost::prior(c), s = p; s != c; t = s++)
   {
      for (FwdIter b = p; b != q; ++b)
      {
         switch (orientation(*t, *s, *b))
         {
         case cg::CG_RIGHT:
            return false;

         case cg::CG_COLLINEAR:
         {
            if (!cg::collinear_are_ordered_along_line(*t, *b, *s))
            {
               return false;
            }

            continue;
         }

         case cg::CG_LEFT:
            continue;
         }
      }
   }

   return true;
}

TEST(convex_hull, simple)
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

   //   std::vector<point_2> pts = boost::assign::list_of(point_2(0, 0))
   //                                                    (point_2(1, 0))
   //                                                    (point_2(0, 1))
   //                                                    (point_2(2, 0))
   //                                                    (point_2(0, 2))
   //                                                    (point_2(3, 0));

   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::graham_hull(pts.begin(), pts.end()), pts.end()));
}

TEST(convex_hull, uniform)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(1000);
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::graham_hull(pts.begin(), pts.end()), pts.end()));
}


TEST(convex_hull, three_with_same)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {0, 0}, {-1, 1}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::graham_hull(pts.begin(), pts.end()), pts.end()));
}

TEST(convex_hull, three_with_same_2)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {1, 0}, {0, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::graham_hull(pts.begin(), pts.end()), pts.end()));
}

TEST(convex_hull, line)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {1, 0}, {2, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::graham_hull(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, simple)
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

   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, simple_2)
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

   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, s1)
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

   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, uniform)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(1000);
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, one)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, two)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {1, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, two_same)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {0, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, three_with_same)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {0, 0}, {-1, 1}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, three_with_same_2)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {1, 0}, {0, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

TEST(andrews_hull, line)
{
   using cg::point_2;

   std::vector<point_2> pts =
   {
      {0, 0}, {1, 0}, {2, 0}
   };
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::andrews(pts.begin(), pts.end()), pts.end()));
}

