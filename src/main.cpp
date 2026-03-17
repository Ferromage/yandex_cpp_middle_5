#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <iterator>
#include <print>
#include <random>
#include <ranges>
#include <span>
#include <variant>

using namespace geometry;

namespace {
template <typename... Ts>
struct Multilambda : Ts... {
    using Ts::operator()...;
};
}  // namespace

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    auto filtered = others | std::views::filter([](const Shape &other) {
                        return std::holds_alternative<Line>(other) || std::holds_alternative<Circle>(other);
                    });

    std::vector<Point2D> points;
    std::ranges::for_each(filtered, [&](const Shape &other) {
        const auto point = geometry::intersections::GetIntersectPoint(shape, other);
        point.and_then([&](const Point2D &point) -> std::optional<Point2D> {
            points.emplace_back(point);
            return {};
        });
    });

    std::println("{}", points);
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    std::vector<Shape> result;
    std::mt19937 gen{std::random_device{}()};
    std::ranges::sample(shapes, std::back_inserter(result), 5, gen);

    std::ranges::for_each(result, [&](const Shape &shape) {
        const auto distance = geometry::queries::DistanceToPoint(shape, p);
        std::visit(
            [p, distance](const auto &item) {
                std::println("Расстояние от точки {} до фигуры {} равно {}", p, item, distance);
            },
            shape);
    });
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    if (shapes.empty()) {
        return;
    }

    std::println("\n=== Shape Analysis ===");

    {
        const auto intersections = geometry::utils::FindAllCollisions(shapes);
        if (!intersections.empty()) {
            std::ranges::for_each(intersections, [&](const auto &item) {
                std::visit([](const auto &shape1,
                              const auto &shape2) { std::println("   intersection: {} {}", shape1, shape2); },
                           item.first, item.second);
            });
        } else {
            std::println("   intersections are not found");
        }
    }

    {
        const auto heightIdx = geometry::utils::FindHighestShape(shapes);
        if (heightIdx) {
            std::visit([](const auto &shape) { std::println("   the highest shape: {}", shape); },
                       shapes.at(heightIdx.value()));
        } else {
            std::println("   the highest shape is not found");
        }
    }

    {
        bool isFound = false;
        for (std::size_t i = 0; i < shapes.size() - 1; i++) {
            for (std::size_t j = i + 1; j < shapes.size(); j++) {
                if (auto dist = geometry::queries::DistanceBetweenShapes(shapes[i], shapes[j]); dist.has_value()) {
                    std::visit(
                        [&](const auto &shape1, const auto &shape2) {
                            std::println("   distance between {} and {}: {}", shape1, shape2, dist.value());
                        },
                        shapes[i], shapes[j]);
                    isFound = true;
                    break;
                }
            }
            if (isFound) {
                break;
            }
        }

        if (!isFound) {
            std::println("   distance calculation is not supported");
        }
    }
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    {
        auto filtered = shapes | std::views::filter([](const Shape &s) {
                            return std::visit([&](const auto &shape) -> bool { return shape.Center().y > 50.0; }, s);
                        });

        std::vector<Shape> result;
        std::mt19937 gen{std::random_device{}()};
        std::ranges::sample(filtered, std::back_inserter(result), 3, gen);

        std::ranges::for_each(result, [&](const Shape &shape) {
            std::visit([](const auto &item) { std::println("   above 50.0: {}", item); }, shape);
        });
    }

    {
        const auto highestIdx = geometry::utils::FindHighestShape(shapes);
        if (highestIdx) {
            std::visit([](const auto &item) { std::println("   the highest shape: {}", item); },
                       shapes.at(highestIdx.value()));
        } else {
            std::println("   the highest shape is not found");
        }

        const auto lowestIdx = geometry::utils::FindLowestShape(shapes);
        if (lowestIdx) {
            std::visit([](const auto &item) { std::println("   the lowest shape: {}", item); },
                       shapes.at(lowestIdx.value()));
        } else {
            std::println("   the lowest shape is not found");
        }
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");

    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту
    std::ranges::for_each(shapes, [&](const Shape &shape) {
        std::visit([&](const auto &s) { std::println("   {}: {}", shape.index(), s.Height()); }, shape);
    });

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;
    for (const auto &shape : shapes) {
        std::visit(Multilambda{[&](const Line &line) {
                                   for (const auto &p : line.Vertices()) {
                                       points.emplace_back(p);
                                   }
                               },
                               [&](const Triangle &tri) {
                                   for (const auto &p : tri.Vertices()) {
                                       points.emplace_back(p);
                                   }
                               },
                               [&](const Rectangle &rect) {
                                   for (const auto &p : rect.Vertices()) {
                                       points.emplace_back(p);
                                   }
                               },
                               [&](const RegularPolygon &poly) {
                                   for (const auto &p : poly.Vertices()) {
                                       points.emplace_back(p);
                                   }
                               },
                               [&](const Circle &circ) {
                                   for (const auto &p : circ.Vertices()) {
                                       points.emplace_back(p);
                                   }
                               },
                               [&](const Polygon &poly) {
                                   for (const auto &p : poly.Vertices()) {
                                       points.emplace_back(p);
                                   }
                               }},
                   shape);
    }

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    const auto grahamPoints = geometry::convex_hull::GrahamScan(points);
    if (grahamPoints.has_value()) {
        shapes.emplace_back(Polygon(grahamPoints.value()));
        geometry::visualization::Draw(shapes);
    } else {
        std::println("Error on GrahamScan(): {}", grahamPoints.error());
    }

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        const std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};
        const auto delaunay = geometry::triangulation::DelaunayTriangulation(points);
        if (delaunay.has_value()) {
            geometry::visualization::Draw(delaunay.value());
        } else {
            std::println("Error on DelaunayTriangulation(): {}", delaunay.error());
        }
        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //
    }
    return 0;
}