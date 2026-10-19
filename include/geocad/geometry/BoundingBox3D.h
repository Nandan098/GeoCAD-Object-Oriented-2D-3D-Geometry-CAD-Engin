#pragma once

#include "geocad/geometry/Point3D.h"

namespace geocad::geometry {

class BoundingBox3D {
public:
    BoundingBox3D();
    BoundingBox3D(const Point3D& minimum, const Point3D& maximum);

    const Point3D& minimum() const;
    const Point3D& maximum() const;

    Point3D center() const;
    bool intersects(const BoundingBox3D& other) const;

private:
    Point3D minimum_;
    Point3D maximum_;
};

} // namespace geocad::geometry
