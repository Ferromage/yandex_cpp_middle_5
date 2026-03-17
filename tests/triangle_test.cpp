#include "geometry.hpp"
#include <gtest/gtest.h>

using namespace geometry;

TEST(TriangleTest, Vertices) {
    Point2D p1(0, 0), p2(4, 0), p3(0, 3);
    Triangle t(p1, p2, p3);

    auto verts = t.Vertices();
    EXPECT_EQ(verts[0], p1);
    EXPECT_EQ(verts[1], p2);
    EXPECT_EQ(verts[2], p3);
}

TEST(TriangleTest, AreaRightTriangle) {
    Point2D p1(0, 0), p2(4, 0), p3(0, 3);
    Triangle t(p1, p2, p3);

    EXPECT_DOUBLE_EQ(t.Area(), 6.0);
}

TEST(TriangleTest, AreaDuplicatePoints) {
    Point2D p1(1, 1), p2(1, 1), p3(5, 5);
    Triangle t(p1, p2, p3);

    EXPECT_DOUBLE_EQ(t.Area(), 0.0);
}

TEST(TriangleTest, BoundingBoxCalculation) {
    Point2D p1(1, 5), p2(10, 2), p3(4, 8);
    Triangle t(p1, p2, p3);

    BoundingBox box = t.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 1.0);
    EXPECT_DOUBLE_EQ(box.max_x, 10.0);
    EXPECT_DOUBLE_EQ(box.min_y, 2.0);
    EXPECT_DOUBLE_EQ(box.max_y, 8.0);
}

TEST(TriangleTest, BoundingBoxNegativeCoords) {
    Point2D p1(-5, -5), p2(-1, -2), p3(-3, -10);
    Triangle t(p1, p2, p3);

    BoundingBox box = t.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -5.0);
    EXPECT_DOUBLE_EQ(box.max_x, -1.0);
    EXPECT_DOUBLE_EQ(box.min_y, -10.0);
    EXPECT_DOUBLE_EQ(box.max_y, -2.0);
}

TEST(TriangleTest, HeightCalculation) {
    Point2D p1(0, 10), p2(5, 2), p3(3, 6);
    Triangle t(p1, p2, p3);

    EXPECT_DOUBLE_EQ(t.Height(), 10.0);

    Triangle t2({0, -5}, {5, -1}, {3, -10});
    EXPECT_DOUBLE_EQ(t2.Height(), -1.0);
}
