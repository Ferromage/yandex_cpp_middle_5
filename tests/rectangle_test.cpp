#include "geometry.hpp"
#include <gtest/gtest.h>

using namespace geometry;

TEST(RectangleTest, Construction) {
    Point2D p(2.0, 3.0);
    Rectangle rect(p, 10.0, 5.0);

    EXPECT_EQ(rect.bottom_left, p);
    EXPECT_DOUBLE_EQ(rect.width, 10.0);
    EXPECT_DOUBLE_EQ(rect.height, 5.0);

    Point2D tr = rect.TopRight();
    EXPECT_DOUBLE_EQ(tr.x, 12.0);
    EXPECT_DOUBLE_EQ(tr.y, 8.0);
}

TEST(RectangleTest, BoundingBoxCalculation) {
    Point2D p(0.0, 0.0);
    Rectangle rect(p, 20.0, 15.0);

    BoundingBox box = rect.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 0.0);
    EXPECT_DOUBLE_EQ(box.min_y, 0.0);
    EXPECT_DOUBLE_EQ(box.max_x, 20.0);
    EXPECT_DOUBLE_EQ(box.max_y, 15.0);
}

TEST(RectangleTest, VerticesOrder) {
    Point2D p(1.0, 1.0);
    Rectangle rect(p, 4.0, 3.0);

    auto verts = rect.Vertices();

    EXPECT_EQ(verts[0], Point2D(1.0, 1.0));
    EXPECT_EQ(verts[1], Point2D(5.0, 1.0));
    EXPECT_EQ(verts[2], Point2D(5.0, 4.0));
    EXPECT_EQ(verts[3], Point2D(1.0, 4.0));
}

TEST(RectangleTest, HeightCalculation) {
    Rectangle r1({0, 0}, 10, 5);
    EXPECT_DOUBLE_EQ(r1.Height(), 5.0);

    Rectangle r2({0, 10}, 10, 5);
    EXPECT_DOUBLE_EQ(r2.Height(), 15.0);

    Rectangle r3({0, -10}, 10, 5);
    EXPECT_DOUBLE_EQ(r3.Height(), -5.0);
}

TEST(RectangleTest, CenterCalculation) {
    Rectangle rect({2.0, 4.0}, 10.0, 6.0);
    Point2D center = rect.Center();

    EXPECT_DOUBLE_EQ(center.x, 7.0);
    EXPECT_DOUBLE_EQ(center.y, 7.0);
}

TEST(RectangleTest, CenterZeroBased) {
    Rectangle rect({0.0, 0.0}, 10.0, 10.0);
    Point2D center = rect.Center();
    EXPECT_DOUBLE_EQ(center.x, 5.0);
    EXPECT_DOUBLE_EQ(center.y, 5.0);
}

TEST(RectangleTest, ZeroDimensions) {
    Rectangle rect({5.0, 5.0}, 0.0, 0.0);

    Point2D tr = rect.TopRight();
    EXPECT_EQ(tr, Point2D(5.0, 5.0));

    auto verts = rect.Vertices();
    for (const auto &v : verts) {
        EXPECT_EQ(v, Point2D(5.0, 5.0));
    }
}