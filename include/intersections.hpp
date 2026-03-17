#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>

namespace geometry::intersections {

namespace {
bool isEqualToZero(double num) { return std::abs(num - 0.0) < 0.000000001; }
}  // namespace

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */
class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Line &line1, const Line &line2) const {
        const auto x1 = line1.start.x;
        const auto y1 = line1.start.y;

        const auto x2 = line1.end.x;
        const auto y2 = line1.end.y;

        const auto x3 = line2.start.x;
        const auto y3 = line2.start.y;

        const auto x4 = line2.end.x;
        const auto y4 = line2.end.y;

        const double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (isEqualToZero(denominator)) {
            return std::nullopt;
        }

        const double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
        const double u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denominator;

        if (0.0 <= t && t <= 1.0 && 0.0 <= u && u <= 1.0) {
            return Point2D(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
        }

        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Circle &circ, const Line &line) const {
        return this->operator()(line, circ);
    }

    std::optional<Point2D> operator()(const Line &line, const Circle &circ) const {
        const auto x1 = line.start.x;
        const auto y1 = line.start.y;

        const auto x2 = line.end.x;
        const auto y2 = line.end.y;

        const auto xc = circ.center_p.x;
        const auto yc = circ.center_p.y;
        const auto r = circ.radius;

        const auto dx = x2 - x1;
        const auto dy = y2 - y1;

        const auto a = std::pow(dx, 2) + std::pow(dy, 2);
        const auto b = 2 * (dx * (x1 - xc) + dy * (y1 - yc));
        const auto c = std::pow(x1 - xc, 2) + std::pow(y1 - yc, 2) - std::pow(r, 2);

        const auto det = std::pow(b, 2) - 4 * a * c;
        std::vector<Point2D> points;

        if (det < 0) {
            return std::nullopt;
        } else if (isEqualToZero(det)) {
            const auto t = -b / (2 * a);
            if (0.0 <= t && t <= 1.0) {
                points.push_back(Point2D(x1 + t * dx, y1 + t * dy));
            }
        } else {
            const auto t1 = (-b + std::sqrt(det)) / (2 * a);
            const auto t2 = (-b - std::sqrt(det)) / (2 * a);

            for (auto t : {t1, t2}) {
                if (0.0 <= t && t <= 1.0) {
                    points.push_back(Point2D(x1 + t * dx, y1 + t * dy));
                }
            }
        }

        if (!points.empty()) {
            return points[0];
        }

        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Circle &circ1, const Circle &circ2) const {
        const auto x1 = circ1.center_p.x;
        const auto y1 = circ1.center_p.y;

        const auto x2 = circ2.center_p.x;
        const auto y2 = circ2.center_p.y;

        const auto r1 = circ1.radius;
        const auto r2 = circ2.radius;

        const auto dx = x2 - x1;
        const auto dy = y2 - y1;
        const auto d = std::hypot(dx, dy);

        if (d > (r1 + r2) || d < std::abs(r1 - r2) || isEqualToZero(d)) {
            return std::nullopt;
        }

        const auto a = (std::pow(r1, 2) - std::pow(r2, 2) + std::pow(d, 2)) / (2 * d);
        const auto h = std::sqrt(std::max(0.0, std::pow(r1, 2) - std::pow(a, 2)));

        const auto x3 = x1 + a * dx / d;
        const auto y3 = y1 + a * dy / d;

        const auto rx = -dy * (h / d);
        const auto ry = dx * (h / d);

        std::vector<Point2D> points;
        points.push_back(Point2D(x3 + rx, y3 + ry));
        if (!isEqualToZero(h)) {
            points.push_back(Point2D(x3 - rx, y3 - ry));
        }

        return points[0];
    }

    std::optional<Point2D> operator()(const auto &, const auto &) const {
        throw std::logic_error("IntersectionVisitor: unsupported type combination");
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(IntersectionVisitor{}, shape1, shape2);
}

}  // namespace geometry::intersections