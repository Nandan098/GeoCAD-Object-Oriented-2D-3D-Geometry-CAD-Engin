#include "geocad/shapes/Sphere.h"
#include "geocad/geometry/Constants.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace geocad::shapes {

Sphere::Sphere(const std::string& id, double x, double y, double z, double radius)
    : Shape(id), center_(x, y, z), radius_(radius)
{
    if (radius <= 0.0) {
        throw std::invalid_argument("Sphere radius must be positive.");
    }
}

std::string Sphere::typeName() const { return "SPHERE"; }
ShapeDimension Sphere::dimension() const { return ShapeDimension::ThreeD; }

double Sphere::area() const { return 4.0 * geocad::geometry::kPi * radius_ * radius_; }
double Sphere::perimeter() const { return 0.0; }
double Sphere::volume() const { return 4.0 * geocad::geometry::kPi * radius_ * radius_ * radius_ / 3.0; }

geocad::geometry::Point3D Sphere::referencePoint() const { return center_; }

geocad::geometry::BoundingBox3D Sphere::boundingBox() const
{
    return geocad::geometry::BoundingBox3D(
        geocad::geometry::Point3D(center_.x() - radius_, center_.y() - radius_, center_.z() - radius_),
        geocad::geometry::Point3D(center_.x() + radius_, center_.y() + radius_, center_.z() + radius_));
}

void Sphere::translate(const geocad::geometry::Vector3D& delta)
{
    center_ = center_ + delta;
}

void Sphere::rotate(double, double, double)
{
    // A sphere is rotationally symmetric, so rotation does not change its geometry.
}

void Sphere::scale(double factor)
{
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    radius_ *= factor;
}

std::unique_ptr<Shape> Sphere::clone() const
{
    return std::make_unique<Sphere>(*this);
}

std::string Sphere::serialize() const
{
    std::ostringstream output;
    output << "SPHERE " << id_ << ' ' << center_.x() << ' ' << center_.y() << ' ' << center_.z() << ' ' << radius_;
    return output.str();
}

const geocad::geometry::Point3D& Sphere::center() const { return center_; }
double Sphere::radius() const { return radius_; }

} // namespace geocad::shapes
