#include "geometry.hpp"
#include <gtest/gtest.h>

using namespace geometry;

namespace {
constexpr double EPSILON = 1e-6;
}

TEST(CircleTest, Center) {
    Point2D center(5.0, -3.0);
    double r = 10.0;
    Circle c(center, r);

    EXPECT_DOUBLE_EQ(c.center_p.x, 5.0);
    EXPECT_DOUBLE_EQ(c.center_p.y, -3.0);
    EXPECT_DOUBLE_EQ(c.radius, 10.0);
    EXPECT_EQ(c.Center(), center);
}

TEST(CircleTest, BoundingBox) {
    Point2D center(2.0, 4.0);
    double r = 3.0;
    Circle c(center, r);

    BoundingBox box = c.BoundBox();

    EXPECT_DOUBLE_EQ(box.min_x, -1.0);
    EXPECT_DOUBLE_EQ(box.min_y, 1.0);
    EXPECT_DOUBLE_EQ(box.max_x, 5.0);
    EXPECT_DOUBLE_EQ(box.max_y, 7.0);
}

TEST(CircleTest, Height) {
    Circle c1({0.0, 0.0}, 5.0);
    EXPECT_DOUBLE_EQ(c1.Height(), 5.0);

    Circle c2({10.0, 10.0}, 2.5);
    EXPECT_DOUBLE_EQ(c2.Height(), 12.5);

    Circle c3({0.0, -5.0}, 5.0);
    EXPECT_DOUBLE_EQ(c3.Height(), 0.0);
}

TEST(CircleTest, VerticesCount) {
    Circle c({0, 0}, 1.0);

    auto v1 = c.Vertices(10);
    EXPECT_EQ(v1.size(), 10);

    auto v2 = c.Vertices(50);
    EXPECT_EQ(v2.size(), 50);

    auto vDefault = c.Vertices();
    EXPECT_EQ(vDefault.size(), 30);
}

TEST(CircleTest, VerticesGeometry) {
    Circle c({0.0, 0.0}, 1.0);
    auto points = c.Vertices(4);

    EXPECT_NEAR(points[0].x, 1.0, EPSILON);
    EXPECT_NEAR(points[0].y, 0.0, EPSILON);

    EXPECT_NEAR(points[1].x, 0.0, EPSILON);
    EXPECT_NEAR(points[1].y, 1.0, EPSILON);

    EXPECT_NEAR(points[2].x, -1.0, EPSILON);
    EXPECT_NEAR(points[2].y, 0.0, EPSILON);

    EXPECT_NEAR(points[3].x, 0.0, EPSILON);
    EXPECT_NEAR(points[3].y, -1.0, EPSILON);
}

TEST(CircleTest, VerticesDistanceFromCenter) {
    Circle c({5.0, 5.0}, 7.0);
    auto points = c.Vertices(20);

    for (const auto &p : points) {
        double dist = std::hypot(p.x - c.center_p.x, p.y - c.center_p.y);
        EXPECT_NEAR(dist, c.radius, EPSILON);
    }
}

TEST(CircleTest, LinesGeometry) {
    Circle c({0.0, 0.0}, 2.0);
    auto lines = c.Lines(4);

    EXPECT_NEAR(lines.x[0], 2.0, EPSILON);
    EXPECT_NEAR(lines.y[0], 0.0, EPSILON);

    EXPECT_NEAR(lines.x[1], 0.0, EPSILON);
    EXPECT_NEAR(lines.y[1], 2.0, EPSILON);

    EXPECT_NEAR(lines.x[4], 2.0, EPSILON);
    EXPECT_NEAR(lines.y[4], 0.0, EPSILON);
}

TEST(CircleTest, ZeroRadius) {
    Circle c({3.0, 4.0}, 0.0);

    auto box = c.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 3.0);
    EXPECT_DOUBLE_EQ(box.max_y, 4.0);

    auto pts = c.Vertices(5);
    for (const auto &p : pts) {
        EXPECT_DOUBLE_EQ(p.x, 3.0);
        EXPECT_DOUBLE_EQ(p.y, 4.0);
    }
}