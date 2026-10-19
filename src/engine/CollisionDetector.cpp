#include "geocad/engine/CollisionDetector.h"

#include "geocad/geometry/Constants.h"
#include "geocad/geometry/Point2D.h"
#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Rectangle.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <typeinfo>
#include <vector>

namespace geocad::engine {

namespace {

struct Vector2 {
    double x{0.0};
    double y{0.0};
};

double dot(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.y * b.y;
}

std::array<geocad::geometry::Point2D, 4> rectangleCorners(const geocad::shapes::Rectangle& rectangle)
{
    const double halfWidth = rectangle.width() / 2.0;
    const double halfHeight = rectangle.height() / 2.0;
    const double radians = geocad::geometry::degreesToRadians(rectangle.rotationDegrees());
    const double c = std::cos(radians);
    const double s = std::sin(radians);

    const std::array<Vector2, 4> local = {{
        {-halfWidth, -halfHeight},
        {halfWidth, -halfHeight},
        {halfWidth, halfHeight},
        {-halfWidth, halfHeight}
    }};

    std::array<geocad::geometry::Point2D, 4> corners;
    for (std::size_t i = 0; i < local.size(); ++i) {
        corners[i] = geocad::geometry::Point2D(
            rectangle.centerX() + local[i].x * c - local[i].y * s,
            rectangle.centerY() + local[i].x * s + local[i].y * c);
    }
    return corners;
}

std::vector<Vector2> rectangleAxes(const std::array<geocad::geometry::Point2D, 4>& corners)
{
    std::vector<Vector2> axes;
    axes.reserve(2);
    for (std::size_t i = 0; i < 2; ++i) {
        const auto& a = corners[i];
        const auto& b = corners[(i + 1) % 4];
        const Vector2 edge{b.x() - a.x(), b.y() - a.y()};
        const double length = std::hypot(edge.x, edge.y);
        axes.push_back({-edge.y / length, edge.x / length});
    }
    return axes;
}

bool separatedOnAxis(const std::array<geocad::geometry::Point2D, 4>& first,
                     const std::array<geocad::geometry::Point2D, 4>& second,
                     const Vector2& axis)
{
    double minFirst = dot({first[0].x(), first[0].y()}, axis);
    double maxFirst = minFirst;
    double minSecond = dot({second[0].x(), second[0].y()}, axis);
    double maxSecond = minSecond;

    for (std::size_t i = 1; i < 4; ++i) {
        const double firstProjection = dot({first[i].x(), first[i].y()}, axis);
        const double secondProjection = dot({second[i].x(), second[i].y()}, axis);
        minFirst = std::min(minFirst, firstProjection);
        maxFirst = std::max(maxFirst, firstProjection);
        minSecond = std::min(minSecond, secondProjection);
        maxSecond = std::max(maxSecond, secondProjection);
    }

    return maxFirst < minSecond - geocad::geometry::kEpsilon
        || maxSecond < minFirst - geocad::geometry::kEpsilon;
}

} // namespace

bool CollisionDetector::collides(const geocad::shapes::Shape& first, const geocad::shapes::Shape& second)
{
    if (first.dimension() != second.dimension()) {
        return first.boundingBox().intersects(second.boundingBox());
    }

    if (dynamic_cast<const geocad::shapes::Circle*>(&first) != nullptr
        && dynamic_cast<const geocad::shapes::Circle*>(&second) != nullptr) {
        return circleCircle(first, second);
    }

    if (dynamic_cast<const geocad::shapes::Circle*>(&first) != nullptr
        && dynamic_cast<const geocad::shapes::Rectangle*>(&second) != nullptr) {
        return circleRectangle(first, second);
    }

    if (dynamic_cast<const geocad::shapes::Rectangle*>(&first) != nullptr
        && dynamic_cast<const geocad::shapes::Circle*>(&second) != nullptr) {
        return circleRectangle(second, first);
    }

    if (dynamic_cast<const geocad::shapes::Rectangle*>(&first) != nullptr
        && dynamic_cast<const geocad::shapes::Rectangle*>(&second) != nullptr) {
        return rectangleRectangle(first, second);
    }

    // For other shapes the AABB serves as a robust broad-phase test.
    return first.boundingBox().intersects(second.boundingBox());
}

bool CollisionDetector::circleCircle(const geocad::shapes::Shape& first, const geocad::shapes::Shape& second)
{
    const auto& a = dynamic_cast<const geocad::shapes::Circle&>(first);
    const auto& b = dynamic_cast<const geocad::shapes::Circle&>(second);
    const double dx = a.centerX() - b.centerX();
    const double dy = a.centerY() - b.centerY();
    const double radiusSum = a.radius() + b.radius();
    return dx * dx + dy * dy <= radiusSum * radiusSum + geocad::geometry::kEpsilon;
}

bool CollisionDetector::circleRectangle(const geocad::shapes::Shape& circleShape, const geocad::shapes::Shape& rectangleShape)
{
    const auto& circle = dynamic_cast<const geocad::shapes::Circle&>(circleShape);
    const auto& rectangle = dynamic_cast<const geocad::shapes::Rectangle&>(rectangleShape);

    const double radians = geocad::geometry::degreesToRadians(rectangle.rotationDegrees());
    const double cosine = std::cos(-radians);
    const double sine = std::sin(-radians);

    const double dx = circle.centerX() - rectangle.centerX();
    const double dy = circle.centerY() - rectangle.centerY();
    const double localX = dx * cosine - dy * sine;
    const double localY = dx * sine + dy * cosine;

    const double halfWidth = rectangle.width() / 2.0;
    const double halfHeight = rectangle.height() / 2.0;
    const double closestX = std::clamp(localX, -halfWidth, halfWidth);
    const double closestY = std::clamp(localY, -halfHeight, halfHeight);
    const double deltaX = localX - closestX;
    const double deltaY = localY - closestY;

    return deltaX * deltaX + deltaY * deltaY <= circle.radius() * circle.radius() + geocad::geometry::kEpsilon;
}

bool CollisionDetector::rectangleRectangle(const geocad::shapes::Shape& first, const geocad::shapes::Shape& second)
{
    const auto& a = dynamic_cast<const geocad::shapes::Rectangle&>(first);
    const auto& b = dynamic_cast<const geocad::shapes::Rectangle&>(second);
    const auto firstCorners = rectangleCorners(a);
    const auto secondCorners = rectangleCorners(b);

    const auto firstAxes = rectangleAxes(firstCorners);
    const auto secondAxes = rectangleAxes(secondCorners);

    for (const auto& axis : firstAxes) {
        if (separatedOnAxis(firstCorners, secondCorners, axis)) {
            return false;
        }
    }
    for (const auto& axis : secondAxes) {
        if (separatedOnAxis(firstCorners, secondCorners, axis)) {
            return false;
        }
    }
    return true;
}

} // namespace geocad::engine
