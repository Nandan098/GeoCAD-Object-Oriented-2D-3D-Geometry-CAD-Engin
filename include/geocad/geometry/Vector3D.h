#pragma once

#include "geocad/geometry/Point3D.h"

namespace geocad::geometry {

class Vector3D {
public:
    Vector3D() = default;
    Vector3D(double x, double y, double z);
    Vector3D(const Point3D& from, const Point3D& to);

    double x() const;
    double y() const;
    double z() const;

    double magnitude() const;
    Vector3D normalized() const;
    double dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;

private:
    double x_{0.0};
    double y_{0.0};
    double z_{0.0};
};

Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs);
Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs);
Vector3D operator*(const Vector3D& vector, double scalar);
Point3D operator+(const Point3D& point, const Vector3D& vector);
Vector3D operator-(const Point3D& lhs, const Point3D& rhs);

} // namespace geocad::geometry
