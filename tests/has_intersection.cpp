#include <gtest/gtest.h>
#include <cg/operations/has_intersection/segment_segment.h>
#include <cg/operations/has_intersection/triangle_segment.h>
#include <cg/operations/has_intersection/rectangle_segment.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/rectangle.h>
#include <misc/random_utils.h>

using namespace cg;
using namespace util;

// -- START segment_intersection --
TEST(segment_intersection, t1) {
    point_2 a1(0, 0), a2(1, 1), a3(0, 1), a4(1, 0);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), true);
}


TEST(segment_intersection, touch) {
    point_2 a1(0, 0), a2(0, 1), a3(0, 1), a4(1, 0);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), true);
}

TEST(segment_intersection, touch1) {
    point_2 a1(0, 0), a2(0, 1), a3(0, 0.5), a4(1, 0);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), true);
}

TEST(segment_intersection, touch2) {
    point_2 a1(0.5, 0), a2(0.5, 0), a3(0, 0), a4(1, 0);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), true);
}


TEST(segment_intersection, line1) {
    point_2 a1(0, 0), a2(2, 0), a3(0, 0), a4(1, 0);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), true);
}

TEST(segment_intersection, line2) {
    point_2 a1(0, 0), a2(2, 0), a3(2, 0), a4(3, 0);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), true);
}

TEST(segment_intersection, line3) {
    point_2 a1(0, 0), a2(1, 1), a3(2, 2), a4(3, 3);

    segment_2 s1(a1, a2), s2(a3, a4);

    EXPECT_EQ(has_intersection(s1, s2), false);
}
// -- END segment_intersection --

// ---- START segment in triangle --
TEST(triangle_test_s, t1)
{
    point_2 p1(0, 0), p2(2, 2), p3(4, 0);

    triangle_2 tr(p1, p2, p3);
    point_2 pt1(1, 1), pt2(2, 0);
    segment_2 s(pt1, pt2);
    EXPECT_EQ(has_intersection(tr, s), true);
}

TEST(triangle_test_s, t2)
{
    point_2 p1(0, 0), p2(2, 2), p3(4, 0);

    triangle_2 tr(p1, p2, p3);
    point_2 pt1(0, 2), pt2(4, 2);
    segment_2 s(pt1, pt2);
    EXPECT_EQ(has_intersection(tr, s), true);
}


TEST(triangle_test_s, t3)
{
    point_2 p1(0, 0), p2(2, 2), p3(4, 0);

    triangle_2 tr(p1, p2, p3);
    point_2 pt1(0, 0), pt2(1, 1);
    segment_2 s(pt1, pt2);
    EXPECT_EQ(has_intersection(tr, s), true);
}

TEST(triangle_test_s, t4)
{
    point_2 p1(0, 0), p2(2, 2), p3(4, 0);

    triangle_2 tr(p1, p2, p3);
    point_2 pt1(10, 0), pt2(10, 1);
    segment_2 s(pt1, pt2);
    EXPECT_EQ(has_intersection(tr, s), false);
}
// -- END segment in triangle --

// -- START rectangle --
TEST(rect_test, t1)
{
    range_d r1(0, 1), r2(0, 3);
    rectangle_2 rect(r1, r2);
    point_2 p1(-1, 0.5), p2(1, 0.5);
    segment_2 s(p1, p2);

    EXPECT_EQ(has_intersection(rect, s), true);
}

TEST(rect_test, t2)
{
    range_d r1(0, 1), r2(0, 3);
    rectangle_2 rect(r1, r2);
    point_2 p1(-1, 0.5), p2(-2, 0.5);
    segment_2 s(p1, p2);

    EXPECT_EQ(has_intersection(rect, s), false);
}


TEST(rect_test, t3)
{
    range_d r1(0, 1), r2(0, 3);
    rectangle_2 rect(r1, r2);
    point_2 p1(0, 0), p2(0, 1);
    segment_2 s(p1, p2);

    EXPECT_EQ(has_intersection(rect, s), true);
}

TEST(rect_test, t4)
{
    range_d r1(0, 1), r2(0, 1);
    rectangle_2 rect(r1, r2);
    point_2 p1(-1, -1), p2(2, 2);
    segment_2 s(p1, p2);

    EXPECT_EQ(has_intersection(rect, s), true);
}

TEST(rect_test, t5)
{
    range_d r1(0, 1), r2(0, 1);
    rectangle_2 rect(r1, r2);
    point_2 p1(0, 0), p2(3, 0);
    segment_2 s(p1, p2);

    EXPECT_EQ(has_intersection(rect, s), true);
}
// -- END rectangle --
