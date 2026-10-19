#pragma once

#include "geocad/geometry/Point3D.h"
#include "geocad/geometry/Vector3D.h"

namespace geocad::geometry {

class Matrix3D {
public:
    Matrix3D();

    static Matrix3D identity();
    static Matrix3D translation(double tx, double ty, double tz);
    static Matrix3D scaling(double sx, double sy, double sz);
    static Matrix3D rotationX(double degrees);
    static Matrix3D rotationY(double degrees);
    static Matrix3D rotationZ(double degrees);
    static Matrix3D eulerRotation(double rx, double ry, double rz);

    Matrix3D operator*(const Matrix3D& other) const;
    Point3D transformPoint(const Point3D& point) const;
    Vector3D transformVector(const Vector3D& vector) const;

private:
    double m_[4][4]{};
};

} // namespace geocad::geometry
