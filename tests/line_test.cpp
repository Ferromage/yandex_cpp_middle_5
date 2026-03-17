#include "geometry.hpp"
#include <gtest/gtest.h>

using namespace geometry;

TEST(LineTest, Vertices) {
    Point2D p1(1.0, 2.0);
    Point2D p2(4.0, 6.0);
    Line line(p1, p2);

    EXPECT_DOUBLE_EQ(line.start.x, 1.0);
    EXPECT_DOUBLE_EQ(line.start.y, 2.0);
    EXPECT_DOUBLE_EQ(line.end.x, 4.0);
    EXPECT_DOUBLE_EQ(line.end.y, 6.0);

    auto verts = line.Vertices();
    EXPECT_DOUBLE_EQ(verts[0].x, 1.0);
    EXPECT_DOUBLE_EQ(verts[1].y, 6.0);
}

TEST(LineTest, Length) {
    Point2D p1(0.0, 0.0);
    Point2D p2(3.0, 4.0);
    Line line(p1, p2);

    EXPECT_DOUBLE_EQ(line.Length(), 5.0);
}

TEST(LineTest, DirectionVector) {
    Point2D p1(0.0, 0.0);
    Point2D p2(3.0, 4.0);
    Line line(p1, p2);

    Point2D dir = line.Direction();
    EXPECT_NEAR(dir.x, 0.6, 1e-9);
    EXPECT_NEAR(dir.y, 0.8, 1e-9);
}

TEST(LineTest, DirectionZeroLength) {
    Point2D p(5.0, 5.0);
    Line line(p, p);

    Point2D dir = line.Direction();
    EXPECT_DOUBLE_EQ(dir.x, 0.0);
    EXPECT_DOUBLE_EQ(dir.y, 0.0);
}

TEST(LineTest, BoundingBoxNormalOrder) {
    Point2D p1(1.0, 5.0);
    Point2D p2(10.0, 2.0);
    Line line(p1, p2);

    BoundingBox box = line.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 1.0);
    EXPECT_DOUBLE_EQ(box.min_y, 2.0);
    EXPECT_DOUBLE_EQ(box.max_x, 10.0);
    EXPECT_DOUBLE_EQ(box.max_y, 5.0);
}

TEST(LineTest, BoundingBoxReversedOrder) {
    Point2D p1(10.0, 10.0);
    Point2D p2(0.0, 0.0);
    Line line(p1, p2);

    BoundingBox box = line.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 0.0);
    EXPECT_DOUBLE_EQ(box.min_y, 0.0);
    EXPECT_DOUBLE_EQ(box.max_x, 10.0);
    EXPECT_DOUBLE_EQ(box.max_y, 10.0);
}

TEST(LineTest, Height) {
    Point2D p1(0.0, 10.0);
    Point2D p2(5.0, 2.0);
    Line line(p1, p2);

    EXPECT_DOUBLE_EQ(line.Height(), 10.0);

    Line line2(p2, p1);
    EXPECT_DOUBLE_EQ(line2.Height(), 10.0);
}

TEST(LineTest, CenterPoint) {
    Point2D p1(0.0, 0.0);
    Point2D p2(4.0, 6.0);
    Line line(p1, p2);

    Point2D center = line.Center();
    EXPECT_DOUBLE_EQ(center.x, 2.0);
    EXPECT_DOUBLE_EQ(center.y, 3.0);
}

TEST(LineTest, CenterNegativeCoords) {
    Point2D p1(-2.0, -4.0);
    Point2D p2(2.0, 4.0);
    Line line(p1, p2);

    Point2D center = line.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.0);
    EXPECT_DOUBLE_EQ(center.y, 0.0);
}
