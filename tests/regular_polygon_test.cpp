#include "geometry.hpp"
#include <gtest/gtest.h>

using namespace geometry;

TEST(RegularPolygonTest, Construction) {
    Point2D center(10.0, 20.0);
    double r = 5.0;
    int sides = 6;

    RegularPolygon poly(center, r, sides);

    EXPECT_EQ(poly.center_p.x, 10.0);
    EXPECT_EQ(poly.center_p.y, 20.0);
    EXPECT_DOUBLE_EQ(poly.radius, 5.0);
    EXPECT_EQ(poly.sides, 6);
    EXPECT_EQ(poly.Center(), center);
}

TEST(RegularPolygonTest, VerticesCount) {
    RegularPolygon tri({0, 0}, 1.0, 3);
    EXPECT_EQ(tri.Vertices().size(), 3);

    RegularPolygon square({0, 0}, 1.0, 4);
    EXPECT_EQ(square.Vertices().size(), 4);

    RegularPolygon hex({0, 0}, 1.0, 6);
    EXPECT_EQ(hex.Vertices().size(), 6);
}

TEST(RegularPolygonTest, BoundingBoxCalculation) {
    RegularPolygon hex({2.0, 3.0}, 4.0, 6);
    BoundingBox box = hex.BoundBox();

    EXPECT_DOUBLE_EQ(box.min_x, -2.0);
    EXPECT_DOUBLE_EQ(box.min_y, -1.0);
    EXPECT_DOUBLE_EQ(box.max_x, 6.0);
    EXPECT_DOUBLE_EQ(box.max_y, 7.0);
}

TEST(RegularPolygonTest, HeightCalculation) {
    RegularPolygon poly({0.0, 0.0}, 10.0, 5);
    EXPECT_DOUBLE_EQ(poly.Height(), 10.0);

    RegularPolygon poly2({5.0, 5.0}, 2.0, 3);
    EXPECT_DOUBLE_EQ(poly2.Height(), 7.0);

    RegularPolygon poly3({0.0, -10.0}, 5.0, 4);
    EXPECT_DOUBLE_EQ(poly3.Height(), -5.0);
}
