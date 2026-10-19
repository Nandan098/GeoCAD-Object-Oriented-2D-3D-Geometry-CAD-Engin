#include "geocad/shapes/Triangle.h"
#include "geocad/geometry/Constants.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace geocad::shapes {

namespace {

double triangleArea(const std::array<geocad::geometry::Point2D, 3>& p)
{
    return std::abs(
        p[0].x() * (p[1].y() - p[2].y())
        + p[1].x() * (p[2].y() - p[0].y())
        + p[2].x() * (p[0].y() - p[1].y())) / 2.0;
}

} // namespace

Triangle::Triangle(const std::string& id, const geocad::geometry::Point2D& a,
                   const geocad::geometry::Point2D& b, const geocad::geometry::Point2D& c)
    : Shape(id), vertices_{a, b, c}
{
    if (triangleArea(vertices_) <= geocad::geometry::kEpsilon) {
        throw std::invalid_argument("Triangle vertices must not be collinear.");
    }
}

std::string Triangle::typeName() const { return "TRIANGLE"; }
ShapeDimension Triangle::dimension() const { return ShapeDimension::TwoD; }
double Triangle::area() const { return triangleArea(vertices_); }

double Triangle::perimeter() const
{
    auto distance = [](const auto& a, const auto& b) {
        return std::hypot(a.x() - b.x(), a.y() - b.y());
    };
    return distance(vertices_[0], vertices_[1])
        + distance(vertices_[1], vertices_[2])
        + distance(vertices_[2], vertices_[0]);
}

double Triangle::volume() const { return 0.0; }

geocad::geometry::Point3D Triangle::referencePoint() const
{
    return geocad::geometry::Point3D(
        (vertices_[0].x() + vertices_[1].x() + vertices_[2].x()) / 3.0,
        (vertices_[0].y() + vertices_[1].y() + vertices_[2].y()) / 3.0,
        0.0);
}

geocad::geometry::BoundingBox3D Triangle::boundingBox() const
{
    double minX = vertices_[0].x();
    double minY = vertices_[0].y();
    double maxX = minX;
    double maxY = minY;
    for (const auto& point : vertices_) {
        minX = std::min(minX, point.x());
        minY = std::min(minY, point.y());
        maxX = std::max(maxX, point.x());
        maxY = std::max(maxY, point.y());
    }
    return geocad::geometry::BoundingBox3D(
        geocad::geometry::Point3D(minX, minY, 0.0),
        geocad::geometry::Point3D(maxX, maxY, 0.0));
}

void Triangle::translate(const geocad::geometry::Vector3D& delta)
{
    for (auto& point : vertices_) {
        point.setX(point.x() + delta.x());
        point.setY(point.y() + delta.y());
    }
}

void Triangle::rotate(double, double, double rzDegrees)
{
    const auto center = referencePoint();
    const double radians = geocad::geometry::degreesToRadians(rzDegrees);
    const double c = std::cos(radians);
    const double s = std::sin(radians);

    for (auto& point : vertices_) {
        const double dx = point.x() - center.x();
        const double dy = point.y() - center.y();
        point.setX(center.x() + dx * c - dy * s);
        point.setY(center.y() + dx * s + dy * c);
    }
}

void Triangle::scale(double factor)
{
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    const auto center = referencePoint();
    for (auto& point : vertices_) {
        point.setX(center.x() + (point.x() - center.x()) * factor);
        point.setY(center.y() + (point.y() - center.y()) * factor);
    }
}

std::unique_ptr<Shape> Triangle::clone() const
{
    return std::make_unique<Triangle>(*this);
}

std::string Triangle::serialize() const
{
    std::ostringstream output;
    output << "TRIANGLE " << id_;
    for (const auto& point : vertices_) {
        output << ' ' << point.x() << ' ' << point.y();
    }
    return output.str();
}

const std::array<geocad::geometry::Point2D, 3>& Triangle::vertices() const
{
    return vertices_;
}

} // namespace geocad::shapes
