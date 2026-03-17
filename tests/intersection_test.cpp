#include "shape_utils.hpp"
#include <gtest/gtest.h>

using namespace geometry;
using namespace geometry::utils;

TEST(FindAllCollisionsTest, EmptyInput) {
    std::vector<Shape> shapes;
    auto result = FindAllCollisions(shapes);
    EXPECT_TRUE(result.empty());
}

TEST(FindAllCollisionsTest, SingleShape) {
    {
        std::vector<Shape> shapes = {Line({0, 0}, {1, 1})};
        auto result = FindAllCollisions(shapes);
        EXPECT_TRUE(result.empty());
    }

    {
        std::vector<Shape> shapes = {Triangle({1, 1}, {2, 2}, {3, 3})};
        auto result = FindAllCollisions(shapes);
        EXPECT_TRUE(result.empty());
    }
}

TEST(FindAllCollisionsTest, TwoShapesNoCollision) {
    {
        std::vector<Shape> shapes = {Line({0, 0}, {1, 1}), Line({2, 0}, {3, 1})};
        auto result = FindAllCollisions(shapes);
        EXPECT_TRUE(result.empty());
    }

    {
        std::vector<Shape> shapes = {Line({0, 0}, {1, 1}), Triangle({2, 0}, {3, 1}, {4, 0})};
        auto result = FindAllCollisions(shapes);
        EXPECT_TRUE(result.empty());
    }
}

TEST(FindAllCollisionsTest, TwoShapesCollision) {
    {
        std::vector<Shape> shapes = {Line({0, 0}, {5, 5}), Line({0, 5}, {5, 0})};
        auto result = FindAllCollisions(shapes);

        ASSERT_EQ(result.size(), 1);
        EXPECT_EQ(result[0].first, shapes[0]);
        EXPECT_EQ(result[0].second, shapes[1]);
    }
}

TEST(FindAllCollisionsTest, ThreeShapesCollision) {
    std::vector<Shape> shapes = {Line({0, 0}, {5, 5}), Line({0, 5}, {5, 0}), Line({3, 0}, {3, 10})};
    auto result = FindAllCollisions(shapes);
    ASSERT_EQ(result.size(), 3);

    EXPECT_EQ(result[0].first, shapes[0]);
    EXPECT_EQ(result[0].second, shapes[1]);
    EXPECT_EQ(result[1].first, shapes[0]);
    EXPECT_EQ(result[1].second, shapes[2]);
    EXPECT_EQ(result[2].first, shapes[1]);
    EXPECT_EQ(result[2].second, shapes[2]);
}
