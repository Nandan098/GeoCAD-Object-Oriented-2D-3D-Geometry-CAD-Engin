#include "geocad/shapes/Circle.h"

#include "geocad/geometry/Constants.h"

#include <cmath>
#include <stdexcept>
#include <sstream>

namespace geocad::shapes {

Circle::Circle(const std::string& id, double x, double y, double radius)
    : Shape(id), center_(x, y), radius_(radius)
{
    if (radius <= 0.0) {
        throw std::invalid_argument("Circle radius must be positive.");
    }
}

std::string Circle::typeName() const { return "CIRCLE"; }
ShapeDimension Circle::dimension() const { return ShapeDimension::TwoD; }

double Circle::area() const { return geocad::geometry::kPi * radius_ * radius_; }
double Circle::perimeter() const { return 2.0 * geocad::geometry::kPi * radius_; }
double Circle::volume() const { return 0.0; }

double Circle::centerX() const { return center_.x(); }
double Circle::centerY() const { return center_.y(); }
double Circle::radius() const { return radius_; }

geocad::geometry::Point3D Circle::referencePoint() const
{
    return geocad::geometry::Point3D(center_.x(), center_.y(), 0.0);
}

geocad::geometry::BoundingBox3D Circle::boundingBox() const
{
    return geocad::geometry::BoundingBox3D(
        geocad::geometry::Point3D(center_.x() - radius_, center_.y() - radius_, 0.0),
        geocad::geometry::Point3D(center_.x() + radius_, center_.y() + radius_, 0.0));
}

void Circle::translate(const geocad::geometry::Vector3D& delta)
{
    center_.setX(center_.x() + delta.x());
    center_.setY(center_.y() + delta.y());
}

void Circle::rotate(double, double, double rzDegrees)
{
    rotationDegrees_ += rzDegrees;
}

void Circle::scale(double factor)
{
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    radius_ *= factor;
}

std::unique_ptr<Shape> Circle::clone() const
{
    return std::make_unique<Circle>(*this);
}

std::string Circle::serialize() const
{
    std::ostringstream output;
    output << "CIRCLE " << id_ << ' ' << center_.x() << ' ' << center_.y() << ' ' << radius_;
    return output.str();
}

} // namespace geocad::shapes
