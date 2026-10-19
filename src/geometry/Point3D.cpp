#include "geocad/geometry/Point3D.h"

namespace geocad::geometry {

Point3D::Point3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}

double Point3D::x() const { return x_; }
double Point3D::y() const { return y_; }
double Point3D::z() const { return z_; }

void Point3D::setX(double x) { x_ = x; }
void Point3D::setY(double y) { y_ = y; }
void Point3D::setZ(double z) { z_ = z; }

} // namespace geocad::geometry
