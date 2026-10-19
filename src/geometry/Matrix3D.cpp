#include "geocad/geometry/Matrix3D.h"
#include "geocad/geometry/Constants.h"

#include <cmath>

namespace geocad::geometry {

Matrix3D::Matrix3D()
{
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            m_[row][column] = (row == column) ? 1.0 : 0.0;
        }
    }
}

Matrix3D Matrix3D::identity()
{
    return Matrix3D();
}

Matrix3D Matrix3D::translation(double tx, double ty, double tz)
{
    Matrix3D matrix;
    matrix.m_[0][3] = tx;
    matrix.m_[1][3] = ty;
    matrix.m_[2][3] = tz;
    return matrix;
}

Matrix3D Matrix3D::scaling(double sx, double sy, double sz)
{
    Matrix3D matrix;
    matrix.m_[0][0] = sx;
    matrix.m_[1][1] = sy;
    matrix.m_[2][2] = sz;
    return matrix;
}

Matrix3D Matrix3D::rotationX(double degrees)
{
    Matrix3D matrix;
    const double radians = degreesToRadians(degrees);
    const double c = std::cos(radians);
    const double s = std::sin(radians);

    matrix.m_[1][1] = c;
    matrix.m_[1][2] = -s;
    matrix.m_[2][1] = s;
    matrix.m_[2][2] = c;
    return matrix;
}

Matrix3D Matrix3D::rotationY(double degrees)
{
    Matrix3D matrix;
    const double radians = degreesToRadians(degrees);
    const double c = std::cos(radians);
    const double s = std::sin(radians);

    matrix.m_[0][0] = c;
    matrix.m_[0][2] = s;
    matrix.m_[2][0] = -s;
    matrix.m_[2][2] = c;
    return matrix;
}

Matrix3D Matrix3D::rotationZ(double degrees)
{
    Matrix3D matrix;
    const double radians = degreesToRadians(degrees);
    const double c = std::cos(radians);
    const double s = std::sin(radians);

    matrix.m_[0][0] = c;
    matrix.m_[0][1] = -s;
    matrix.m_[1][0] = s;
    matrix.m_[1][1] = c;
    return matrix;
}

Matrix3D Matrix3D::eulerRotation(double rx, double ry, double rz)
{
    return rotationZ(rz) * rotationY(ry) * rotationX(rx);
}

Matrix3D Matrix3D::operator*(const Matrix3D& other) const
{
    Matrix3D result;
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            result.m_[row][column] = 0.0;
            for (int inner = 0; inner < 4; ++inner) {
                result.m_[row][column] += m_[row][inner] * other.m_[inner][column];
            }
        }
    }
    return result;
}

Point3D Matrix3D::transformPoint(const Point3D& point) const
{
    const double x = m_[0][0] * point.x() + m_[0][1] * point.y() + m_[0][2] * point.z() + m_[0][3];
    const double y = m_[1][0] * point.x() + m_[1][1] * point.y() + m_[1][2] * point.z() + m_[1][3];
    const double z = m_[2][0] * point.x() + m_[2][1] * point.y() + m_[2][2] * point.z() + m_[2][3];
    const double w = m_[3][0] * point.x() + m_[3][1] * point.y() + m_[3][2] * point.z() + m_[3][3];

    if (std::abs(w) > kEpsilon && !approximatelyEqual(w, 1.0)) {
        return Point3D(x / w, y / w, z / w);
    }
    return Point3D(x, y, z);
}

Vector3D Matrix3D::transformVector(const Vector3D& vector) const
{
    const double x = m_[0][0] * vector.x() + m_[0][1] * vector.y() + m_[0][2] * vector.z();
    const double y = m_[1][0] * vector.x() + m_[1][1] * vector.y() + m_[1][2] * vector.z();
    const double z = m_[2][0] * vector.x() + m_[2][1] * vector.y() + m_[2][2] * vector.z();
    return Vector3D(x, y, z);
}

} // namespace geocad::geometry
