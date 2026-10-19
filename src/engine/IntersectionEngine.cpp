#include "geocad/engine/IntersectionEngine.h"
#include "geocad/geometry/Constants.h"

#include <algorithm>
#include <cmath>

namespace geocad::engine {

LineIntersectionResult IntersectionEngine::intersectLines(const geocad::geometry::Line2D& first, const geocad::geometry::Line2D& second)
{
    const double x1 = first.start().x();
    const double y1 = first.start().y();
    const double x2 = first.end().x();
    const double y2 = first.end().y();
    const double x3 = second.start().x();
    const double y3 = second.start().y();
    const double x4 = second.end().x();
    const double y4 = second.end().y();

    const double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (std::abs(denominator) <= geocad::geometry::kEpsilon) {
        const double cross = (x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1);
        if (std::abs(cross) <= geocad::geometry::kEpsilon) {
            return {LineIntersectionType::Coincident, std::nullopt};
        }
        return {LineIntersectionType::Parallel, std::nullopt};
    }

    const double determinant1 = x1 * y2 - y1 * x2;
    const double determinant2 = x3 * y4 - y3 * x4;
    const double x = (determinant1 * (x3 - x4) - (x1 - x2) * determinant2) / denominator;
    const double y = (determinant1 * (y3 - y4) - (y1 - y2) * determinant2) / denominator;

    const auto within = [](double value, double low, double high) {
        return value >= std::min(low, high) - geocad::geometry::kEpsilon
            && value <= std::max(low, high) + geocad::geometry::kEpsilon;
    };

    if (!within(x, x1, x2) || !within(y, y1, y2)
        || !within(x, x3, x4) || !within(y, y3, y4)) {
        return {LineIntersectionType::None, std::nullopt};
    }

    return {LineIntersectionType::Point, geocad::geometry::Point2D(x, y)};
}

std::vector<geocad::geometry::Point2D> IntersectionEngine::intersectLineCircle(const geocad::geometry::Line2D& line, const geocad::shapes::Circle& circle)
{
    const double x1 = line.start().x() - circle.centerX();
    const double y1 = line.start().y() - circle.centerY();
    const double dx = line.end().x() - line.start().x();
    const double dy = line.end().y() - line.start().y();

    const double a = dx * dx + dy * dy;
    if (a <= geocad::geometry::kEpsilon) {
        return {};
    }

    const double b = 2.0 * (x1 * dx + y1 * dy);
    const double c = x1 * x1 + y1 * y1 - circle.radius() * circle.radius();
    const double discriminant = b * b - 4.0 * a * c;

    if (discriminant < -geocad::geometry::kEpsilon) {
        return {};
    }

    const double clampedDiscriminant = std::max(0.0, discriminant);
    const double sqrtDiscriminant = std::sqrt(clampedDiscriminant);
    const double t1 = (-b - sqrtDiscriminant) / (2.0 * a);
    const double t2 = (-b + sqrtDiscriminant) / (2.0 * a);

    std::vector<geocad::geometry::Point2D> result;
    const auto addIfOnSegment = [&](double t) {
        if (t < -geocad::geometry::kEpsilon || t > 1.0 + geocad::geometry::kEpsilon) {
            return;
        }
        result.emplace_back(line.start().x() + t * dx, line.start().y() + t * dy);
    };

    addIfOnSegment(t1);
    if (!geocad::geometry::approximatelyEqual(t1, t2)) {
        addIfOnSegment(t2);
    }
    return result;
}

std::vector<geocad::geometry::Point2D> IntersectionEngine::intersectCircles(const geocad::shapes::Circle& first, const geocad::shapes::Circle& second)
{
    const double dx = second.centerX() - first.centerX();
    const double dy = second.centerY() - first.centerY();
    const double d = std::hypot(dx, dy);

    if (d <= geocad::geometry::kEpsilon) {
        return {};
    }
    if (d > first.radius() + second.radius() + geocad::geometry::kEpsilon) {
        return {};
    }
    if (d < std::abs(first.radius() - second.radius()) - geocad::geometry::kEpsilon) {
        return {};
    }

    const double a = (first.radius() * first.radius() - second.radius() * second.radius() + d * d) / (2.0 * d);
    const double hSquared = first.radius() * first.radius() - a * a;
    if (hSquared < -geocad::geometry::kEpsilon) {
        return {};
    }

    const double h = std::sqrt(std::max(0.0, hSquared));
    const double baseX = first.centerX() + a * dx / d;
    const double baseY = first.centerY() + a * dy / d;
    const double offsetX = -dy * h / d;
    const double offsetY = dx * h / d;

    std::vector<geocad::geometry::Point2D> result;
    result.emplace_back(baseX + offsetX, baseY + offsetY);
    if (h > geocad::geometry::kEpsilon) {
        result.emplace_back(baseX - offsetX, baseY - offsetY);
    }
    return result;
}

bool IntersectionEngine::pointInPolygon(const geocad::geometry::Point2D& point, const geocad::shapes::Polygon& polygon)
{
    bool inside = false;
    const auto& vertices = polygon.vertices();

    for (std::size_t i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
        const auto& a = vertices[i];
        const auto& b = vertices[j];
        const bool intersects = ((a.y() > point.y()) != (b.y() > point.y()))
            && (point.x() < (b.x() - a.x()) * (point.y() - a.y()) / (b.y() - a.y()) + a.x());
        if (intersects) {
            inside = !inside;
        }
    }
    return inside;
}

} // namespace geocad::engine
