#include "geocad/shapes/Cylinder.h"
#include "geocad/geometry/Constants.h"
#include "geocad/geometry/Matrix3D.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace geocad::shapes {

Cylinder::Cylinder(const std::string& id, double x, double y, double z, double radius, double height,
                   double rxDegrees, double ryDegrees, double rzDegrees)
    : Shape(id), center_(x, y, z), radius_(radius), height_(height),
      rxDegrees_(rxDegrees), ryDegrees_(ryDegrees), rzDegrees_(rzDegrees)
{
    if (radius <= 0.0 || height <= 0.0) {
        throw std::invalid_argument("Cylinder radius and height must be positive.");
    }
}

std::string Cylinder::typeName() const { return "CYLINDER"; }
ShapeDimension Cylinder::dimension() const { return ShapeDimension::ThreeD; }

double Cylinder::area() const
{
    return 2.0 * geocad::geometry::kPi * radius_ * (radius_ + height_);
}

double Cylinder::perimeter() const { return 0.0; }

double Cylinder::volume() const
{
    return geocad::geometry::kPi * radius_ * radius_ * height_;
}

geocad::geometry::Point3D Cylinder::referencePoint() const { return center_; }

geocad::geometry::BoundingBox3D Cylinder::boundingBox() const
{
    const double halfHeight = height_ / 2.0;
    const auto rotation = geocad::geometry::Matrix3D::eulerRotation(rxDegrees_, ryDegrees_, rzDegrees_);

    const double localValues[2] = {-1.0, 1.0};
    double minX = 1e300;
    double minY = 1e300;
    double minZ = 1e300;
    double maxX = -1e300;
    double maxY = -1e300;
    double maxZ = -1e300;

    // A conservative local box enclosing the cylinder. The exact circular boundary is unnecessary for AABB broad-phase collision.
    for (double xScale : localValues) {
        for (double yScale : localValues) {
            for (double zScale : localValues) {
                const auto rotated = rotation.transformPoint(
                    geocad::geometry::Point3D(xScale * radius_, yScale * radius_, zScale * halfHeight));
                minX = std::min(minX, rotated.x() + center_.x());
                minY = std::min(minY, rotated.y() + center_.y());
                minZ = std::min(minZ, rotated.z() + center_.z());
                maxX = std::max(maxX, rotated.x() + center_.x());
                maxY = std::max(maxY, rotated.y() + center_.y());
                maxZ = std::max(maxZ, rotated.z() + center_.z());
            }
        }
    }

    return geocad::geometry::BoundingBox3D(
        geocad::geometry::Point3D(minX, minY, minZ),
        geocad::geometry::Point3D(maxX, maxY, maxZ));
}

void Cylinder::translate(const geocad::geometry::Vector3D& delta)
{
    center_ = center_ + delta;
}

void Cylinder::rotate(double rxDegrees, double ryDegrees, double rzDegrees)
{
    rxDegrees_ += rxDegrees;
    ryDegrees_ += ryDegrees;
    rzDegrees_ += rzDegrees;
}

void Cylinder::scale(double factor)
{
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    radius_ *= factor;
    height_ *= factor;
}

std::unique_ptr<Shape> Cylinder::clone() const
{
    return std::make_unique<Cylinder>(*this);
}

std::string Cylinder::serialize() const
{
    std::ostringstream output;
    output << "CYLINDER " << id_ << ' ' << center_.x() << ' ' << center_.y() << ' ' << center_.z() << ' '
           << radius_ << ' ' << height_ << ' ' << rxDegrees_ << ' ' << ryDegrees_ << ' ' << rzDegrees_;
    return output.str();
}

const geocad::geometry::Point3D& Cylinder::center() const { return center_; }
double Cylinder::radius() const { return radius_; }
double Cylinder::height() const { return height_; }
double Cylinder::rotationX() const { return rxDegrees_; }
double Cylinder::rotationY() const { return ryDegrees_; }
double Cylinder::rotationZ() const { return rzDegrees_; }

} // namespace geocad::shapes
