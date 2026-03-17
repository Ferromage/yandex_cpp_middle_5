#include "geometry.hpp"
#include <gtest/gtest.h>

using namespace geometry;

TEST(PolygonTest, RectangleVertices) {
    std::vector<Point2D> pts = {{0, 0}, {4, 0}, {4, 3}, {0, 3}};
    Polygon poly(pts);

    auto vertices = poly.Vertices();
    EXPECT_EQ(vertices.size(), 4);
    EXPECT_EQ(vertices[0], Point2D(0, 0));
    EXPECT_EQ(vertices[2], Point2D(4, 3));
}

TEST(PolygonTest, BoundingBoxRectangle) {
    std::vector<Point2D> pts = {{1, 1}, {5, 1}, {5, 4}, {1, 4}};
    Polygon poly(pts);

    BoundingBox box = poly.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 1.0);
    EXPECT_DOUBLE_EQ(box.min_y, 1.0);
    EXPECT_DOUBLE_EQ(box.max_x, 5.0);
    EXPECT_DOUBLE_EQ(box.max_y, 4.0);
}

TEST(PolygonTest, BoundingBoxUnorderedPoints) {
    std::vector<Point2D> pts = {{10, 10}, {-5, 2}, {0, -10}, {3, 5}};
    Polygon poly(pts);

    BoundingBox box = poly.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -5.0);
    EXPECT_DOUBLE_EQ(box.min_y, -10.0);
    EXPECT_DOUBLE_EQ(box.max_x, 10.0);
    EXPECT_DOUBLE_EQ(box.max_y, 10.0);
}

TEST(PolygonTest, HeightCalculation) {
    std::vector<Point2D> pts = {{0, 0}, {10, 0}, {5, 8}};
    Polygon poly(pts);

    EXPECT_DOUBLE_EQ(poly.Height(), 8.0);

    std::vector<Point2D> pts2 = {{0, -5}, {10, 5}};
    Polygon poly2(pts2);
    EXPECT_DOUBLE_EQ(poly2.Height(), 10.0);
}

TEST(PolygonTest, CenterCalculation) {
    std::vector<Point2D> pts = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    Polygon poly(pts);

    Point2D center = poly.Center();
    EXPECT_DOUBLE_EQ(center.x, 5.0);
    EXPECT_DOUBLE_EQ(center.y, 5.0);

    std::vector<Point2D> pts2 = {{2, 2}, {6, 2}, {6, 8}, {2, 8}};
    Polygon poly2(pts2);
    Point2D center2 = poly2.Center();
    EXPECT_DOUBLE_EQ(center2.x, 4.0);
    EXPECT_DOUBLE_EQ(center2.y, 5.0);
}

TEST(PolygonTest, NegativeCoordinates) {
    std::vector<Point2D> pts = {{-10, -10}, {-2, -5}, {-5, -2}};
    Polygon poly(pts);

    BoundingBox box = poly.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -10.0);
    EXPECT_DOUBLE_EQ(box.max_x, -2.0);
    EXPECT_DOUBLE_EQ(box.min_y, -10.0);
    EXPECT_DOUBLE_EQ(box.max_y, -2.0);
}