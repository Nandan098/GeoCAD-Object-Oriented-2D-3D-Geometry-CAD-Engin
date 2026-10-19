#include "geocad/engine/GeometryEngine.h"
#include "geocad/geometry/Constants.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace geocad::engine {

double GeometryEngine::distance(const geocad::geometry::Point2D& a, const geocad::geometry::Point2D& b)
{
    return std::hypot(a.x() - b.x(), a.y() - b.y());
}

double GeometryEngine::distance(const geocad::geometry::Point3D& a, const geocad::geometry::Point3D& b)
{
    const double dx = a.x() - b.x();
    const double dy = a.y() - b.y();
    const double dz = a.z() - b.z();
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

template <typename Vector>
double angleBetween(const Vector& a, const Vector& b)
{
    const double magnitudeProduct = a.magnitude() * b.magnitude();
    if (magnitudeProduct <= geocad::geometry::kEpsilon) {
        throw std::invalid_argument("Angle is undefined for a zero-length vector.");
    }

    const double cosine = std::clamp(a.dot(b) / magnitudeProduct, -1.0, 1.0);
    return geocad::geometry::radiansToDegrees(std::acos(cosine));
}

double GeometryEngine::angleDegrees(const geocad::geometry::Vector2D& a, const geocad::geometry::Vector2D& b)
{
    return angleBetween(a, b);
}

double GeometryEngine::angleDegrees(const geocad::geometry::Vector3D& a, const geocad::geometry::Vector3D& b)
{
    return angleBetween(a, b);
}

double GeometryEngine::pointToLineDistance(const geocad::geometry::Point2D& point, const geocad::geometry::Line2D& line)
{
    const double x1 = line.start().x();
    const double y1 = line.start().y();
    const double x2 = line.end().x();
    const double y2 = line.end().y();
    const double numerator = std::abs((y2 - y1) * point.x() - (x2 - x1) * point.y() + x2 * y1 - y2 * x1);
    const double denominator = std::hypot(y2 - y1, x2 - x1);
    if (denominator <= geocad::geometry::kEpsilon) {
        throw std::invalid_argument("Cannot calculate distance to a zero-length line.");
    }
    return numerator / denominator;
}

} // namespace geocad::engine
