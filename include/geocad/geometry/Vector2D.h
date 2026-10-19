#pragma once

#include "geocad/geometry/Point2D.h"

namespace geocad::geometry {

class Vector2D {
public:
    Vector2D() = default;
    Vector2D(double x, double y);
    Vector2D(const Point2D& from, const Point2D& to);

    double x() const;
    double y() const;

    double magnitude() const;
    Vector2D normalized() const;
    double dot(const Vector2D& other) const;
    double cross(const Vector2D& other) const;

private:
    double x_{0.0};
    double y_{0.0};
};

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator*(const Vector2D& vector, double scalar);
Point2D operator+(const Point2D& point, const Vector2D& vector);
Vector2D operator-(const Point2D& lhs, const Point2D& rhs);

} // namespace geocad::geometry
