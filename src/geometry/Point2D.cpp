#include "geocad/geometry/Point2D.h"

namespace geocad::geometry {

Point2D::Point2D(double x, double y) : x_(x), y_(y) {}

double Point2D::x() const { return x_; }
double Point2D::y() const { return y_; }

void Point2D::setX(double x) { x_ = x; }
void Point2D::setY(double y) { y_ = y; }

} // namespace geocad::geometry
