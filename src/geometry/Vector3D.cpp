#include "geocad/geometry/Vector3D.h"
#include "geocad/geometry/Constants.h"

#include <cmath>
#include <stdexcept>

namespace geocad::geometry {

Vector3D::Vector3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}

Vector3D::Vector3D(const Point3D& from, const Point3D& to)
    : x_(to.x() - from.x()), y_(to.y() - from.y()), z_(to.z() - from.z()) {}

double Vector3D::x() const { return x_; }
double Vector3D::y() const { return y_; }
double Vector3D::z() const { return z_; }

double Vector3D::magnitude() const
{
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vector3D Vector3D::normalized() const
{
    const double length = magnitude();
    if (length <= kEpsilon) {
        throw std::invalid_argument("Cannot normalize a zero-length vector.");
    }
    return Vector3D(x_ / length, y_ / length, z_ / length);
}

double Vector3D::dot(const Vector3D& other) const
{
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

Vector3D Vector3D::cross(const Vector3D& other) const
{
    return Vector3D(
        y_ * other.z_ - z_ * other.y_,
        z_ * other.x_ - x_ * other.z_,
        x_ * other.y_ - y_ * other.x_);
}

Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
{
    return Vector3D(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
{
    return Vector3D(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

Vector3D operator*(const Vector3D& vector, double scalar)
{
    return Vector3D(vector.x() * scalar, vector.y() * scalar, vector.z() * scalar);
}

Point3D operator+(const Point3D& point, const Vector3D& vector)
{
    return Point3D(point.x() + vector.x(), point.y() + vector.y(), point.z() + vector.z());
}

Vector3D operator-(const Point3D& lhs, const Point3D& rhs)
{
    return Vector3D(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

} // namespace geocad::geometry
