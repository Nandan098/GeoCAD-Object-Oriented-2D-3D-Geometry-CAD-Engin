#include "geocad/shapes/Cube.h"
#include "geocad/geometry/Matrix3D.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace geocad::shapes {

Cube::Cube(const std::string& id, double x, double y, double z, double side,
           double rxDegrees, double ryDegrees, double rzDegrees)
    : Shape(id), center_(x, y, z), side_(side), rxDegrees_(rxDegrees), ryDegrees_(ryDegrees), rzDegrees_(rzDegrees)
{
    if (side <= 0.0) {
        throw std::invalid_argument("Cube side must be positive.");
    }
}

std::string Cube::typeName() const { return "CUBE"; }
ShapeDimension Cube::dimension() const { return ShapeDimension::ThreeD; }
double Cube::area() const { return 6.0 * side_ * side_; }
double Cube::perimeter() const { return 0.0; }
double Cube::volume() const { return side_ * side_ * side_; }

geocad::geometry::Point3D Cube::referencePoint() const { return center_; }

geocad::geometry::BoundingBox3D Cube::boundingBox() const
{
    const double half = side_ / 2.0;
    const auto rotation = geocad::geometry::Matrix3D::eulerRotation(rxDegrees_, ryDegrees_, rzDegrees_);

    const double values[2] = {-half, half};
    double minX = 1e300;
    double minY = 1e300;
    double minZ = 1e300;
    double maxX = -1e300;
    double maxY = -1e300;
    double maxZ = -1e300;

    for (double x : values) {
        for (double y : values) {
            for (double z : values) {
                const auto rotated = rotation.transformPoint(geocad::geometry::Point3D(x, y, z));
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

void Cube::translate(const geocad::geometry::Vector3D& delta)
{
    center_ = center_ + delta;
}

void Cube::rotate(double rxDegrees, double ryDegrees, double rzDegrees)
{
    rxDegrees_ += rxDegrees;
    ryDegrees_ += ryDegrees;
    rzDegrees_ += rzDegrees;
}

void Cube::scale(double factor)
{
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    side_ *= factor;
}

std::unique_ptr<Shape> Cube::clone() const
{
    return std::make_unique<Cube>(*this);
}

std::string Cube::serialize() const
{
    std::ostringstream output;
    output << "CUBE " << id_ << ' ' << center_.x() << ' ' << center_.y() << ' ' << center_.z() << ' '
           << side_ << ' ' << rxDegrees_ << ' ' << ryDegrees_ << ' ' << rzDegrees_;
    return output.str();
}

const geocad::geometry::Point3D& Cube::center() const { return center_; }
double Cube::side() const { return side_; }
double Cube::rotationX() const { return rxDegrees_; }
double Cube::rotationY() const { return ryDegrees_; }
double Cube::rotationZ() const { return rzDegrees_; }

} // namespace geocad::shapes
