#include "geocad/geometry/Vector2D.h"
#include "geocad/geometry/Constants.h"

#include <cmath>
#include <stdexcept>

namespace geocad::geometry {

Vector2D::Vector2D(double x, double y) : x_(x), y_(y) {}

Vector2D::Vector2D(const Point2D& from, const Point2D& to)
    : x_(to.x() - from.x()), y_(to.y() - from.y()) {}

double Vector2D::x() const { return x_; }
double Vector2D::y() const { return y_; }

double Vector2D::magnitude() const
{
    return std::hypot(x_, y_);
}

Vector2D Vector2D::normalized() const
{
    const double length = magnitude();
    if (length <= kEpsilon) {
        throw std::invalid_argument("Cannot normalize a zero-length vector.");
    }
    return Vector2D(x_ / length, y_ / length);
}

double Vector2D::dot(const Vector2D& other) const
{
    return x_ * other.x_ + y_ * other.y_;
}

double Vector2D::cross(const Vector2D& other) const
{
    return x_ * other.y_ - y_ * other.x_;
}

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
    return Vector2D(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
    return Vector2D(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

Vector2D operator*(const Vector2D& vector, double scalar)
{
    return Vector2D(vector.x() * scalar, vector.y() * scalar);
}

Point2D operator+(const Point2D& point, const Vector2D& vector)
{
    return Point2D(point.x() + vector.x(), point.y() + vector.y());
}

Vector2D operator-(const Point2D& lhs, const Point2D& rhs)
{
    return Vector2D(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

} // namespace geocad::geometry
