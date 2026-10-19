#include "geocad/shapes/Rectangle.h"
#include "geocad/geometry/Constants.h"
#include "geocad/geometry/Matrix3D.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <sstream>

namespace geocad::shapes {

Rectangle::Rectangle(const std::string& id, double centerX, double centerY, double width, double height, double rotationDegrees)
    : Shape(id), center_(centerX, centerY), width_(width), height_(height), rotationDegrees_(rotationDegrees)
{
    if (width <= 0.0 || height <= 0.0) {
        throw std::invalid_argument("Rectangle width and height must be positive.");
    }
}

std::string Rectangle::typeName() const { return "RECTANGLE"; }
ShapeDimension Rectangle::dimension() const { return ShapeDimension::TwoD; }
double Rectangle::area() const { return width_ * height_; }
double Rectangle::perimeter() const { return 2.0 * (width_ + height_); }
double Rectangle::volume() const { return 0.0; }

double Rectangle::centerX() const { return center_.x(); }
double Rectangle::centerY() const { return center_.y(); }
double Rectangle::width() const { return width_; }
double Rectangle::height() const { return height_; }
double Rectangle::rotationDegrees() const { return rotationDegrees_; }

geocad::geometry::Point3D Rectangle::referencePoint() const
{
    return geocad::geometry::Point3D(center_.x(), center_.y(), 0.0);
}

geocad::geometry::BoundingBox3D Rectangle::boundingBox() const
{
    const double halfWidth = width_ / 2.0;
    const double halfHeight = height_ / 2.0;
    const double radians = geocad::geometry::degreesToRadians(rotationDegrees_);
    const double c = std::cos(radians);
    const double s = std::sin(radians);

    double minX = 1e300;
    double minY = 1e300;
    double maxX = -1e300;
    double maxY = -1e300;

    const double localX[4] = {-halfWidth, halfWidth, halfWidth, -halfWidth};
    const double localY[4] = {-halfHeight, -halfHeight, halfHeight, halfHeight};

    for (int i = 0; i < 4; ++i) {
        const double x = center_.x() + localX[i] * c - localY[i] * s;
        const double y = center_.y() + localX[i] * s + localY[i] * c;
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
    }

    return geocad::geometry::BoundingBox3D(
        geocad::geometry::Point3D(minX, minY, 0.0),
        geocad::geometry::Point3D(maxX, maxY, 0.0));
}

void Rectangle::translate(const geocad::geometry::Vector3D& delta)
{
    center_.setX(center_.x() + delta.x());
    center_.setY(center_.y() + delta.y());
}

void Rectangle::rotate(double, double, double rzDegrees)
{
    rotationDegrees_ += rzDegrees;
}

void Rectangle::scale(double factor)
{
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive.");
    }
    width_ *= factor;
    height_ *= factor;
}

std::unique_ptr<Shape> Rectangle::clone() const
{
    return std::make_unique<Rectangle>(*this);
}

std::string Rectangle::serialize() const
{
    std::ostringstream output;
    output << "RECTANGLE " << id_ << ' ' << center_.x() << ' ' << center_.y() << ' '
           << width_ << ' ' << height_ << ' ' << rotationDegrees_;
    return output.str();
}

} // namespace geocad::shapes
