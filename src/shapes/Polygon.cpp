#include "geocad/shapes/Polygon.h"
#include "geocad/geometry/Constants.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace geocad::shapes {

Polygon::Polygon(const std::string& id, const std::vector<geocad::geometry::Point2D>& vertices)
    : Shape(id), vertices_(vertices)
{
    if (vertices_.size() < 3) {
        throw std::invalid_argument("Polygon must have at least three vertices.");
    }
    if (area() <= geocad::geometry::kEpsilon) {
        throw std::invalid_argument("Polygon area must be positive.");
    }
}

std::string Polygon::typeName() const { return "POLYGON"; }
ShapeDimension Polygon::dimension() const { return ShapeDimension::TwoD; }

double Polygon::area() const
{
    double twiceArea = 0.0;
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
        const auto& a = vertices_[i];
        const auto& b = vertices_[(i + 1) % vertices_.size()];
        twiceArea += a.x() * b.y() - b.x() * a.y();
    }
    return std::abs(twiceArea) / 2.0;
}

double Polygon::perimeter() const
{
    double total = 0.0;
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
        const auto& a = vertices_[i];
        const auto& b = vertices_[(i + 1) % vertices_.size()];
        total += std::hypot(a.x() - b.x(), a.y() - b.y());
    }
    return total;
}

double Polygon::volume() const { return 0.0; }

geocad::geometry::Point3D Polygon::referencePoint() const
{
    double x = 0.0;
    double y = 0.0;
    for (const auto& point : vertices_) {
        x += point.x();
        y += point.y();
    }
    const double count = static_cast<double>(vertices_.size());
    return geocad::geometry::Point3D(x / count, y / count, 0.0);
}

geocad::geometry::BoundingBox3D Polygon::boundingBox() const
{
    double minX = vertices_.front().x();
    double minY = vertices_.front().y();
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

void Polygon::translate(const geocad::geometry::Vector3D& delta)
{
    for (auto& point : vertices_) {
        point.setX(point.x() + delta.x());
        point.setY(point.y() + delta.y());
    }
}

void Polygon::rotate(double, double, double rzDegrees)
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

void Polygon::scale(double factor)
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

std::unique_ptr<Shape> Polygon::clone() const
{
    return std::make_unique<Polygon>(*this);
}

std::string Polygon::serialize() const
{
    std::ostringstream output;
    output << "POLYGON " << id_ << ' ' << vertices_.size();
    for (const auto& point : vertices_) {
        output << ' ' << point.x() << ' ' << point.y();
    }
    return output.str();
}

const std::vector<geocad::geometry::Point2D>& Polygon::vertices() const
{
    return vertices_;
}

} // namespace geocad::shapes
