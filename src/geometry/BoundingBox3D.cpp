#include "geocad/geometry/BoundingBox3D.h"

namespace geocad::geometry {

BoundingBox3D::BoundingBox3D() = default;

BoundingBox3D::BoundingBox3D(const Point3D& minimum, const Point3D& maximum)
    : minimum_(minimum), maximum_(maximum) {}

const Point3D& BoundingBox3D::minimum() const { return minimum_; }
const Point3D& BoundingBox3D::maximum() const { return maximum_; }

Point3D BoundingBox3D::center() const
{
    return Point3D(
        (minimum_.x() + maximum_.x()) / 2.0,
        (minimum_.y() + maximum_.y()) / 2.0,
        (minimum_.z() + maximum_.z()) / 2.0);
}

bool BoundingBox3D::intersects(const BoundingBox3D& other) const
{
    return minimum_.x() <= other.maximum_.x() && maximum_.x() >= other.minimum_.x()
        && minimum_.y() <= other.maximum_.y() && maximum_.y() >= other.minimum_.y()
        && minimum_.z() <= other.maximum_.z() && maximum_.z() >= other.minimum_.z();
}

} // namespace geocad::geometry
