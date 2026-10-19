#pragma once

#include "geocad/geometry/Line2D.h"
#include "geocad/geometry/Point2D.h"
#include "geocad/geometry/Point3D.h"
#include "geocad/geometry/Vector2D.h"
#include "geocad/geometry/Vector3D.h"

namespace geocad::engine {

class GeometryEngine {
public:
    static double distance(const geocad::geometry::Point2D& a, const geocad::geometry::Point2D& b);
    static double distance(const geocad::geometry::Point3D& a, const geocad::geometry::Point3D& b);
    static double angleDegrees(const geocad::geometry::Vector2D& a, const geocad::geometry::Vector2D& b);
    static double angleDegrees(const geocad::geometry::Vector3D& a, const geocad::geometry::Vector3D& b);
    static double pointToLineDistance(const geocad::geometry::Point2D& point, const geocad::geometry::Line2D& line);
};

} // namespace geocad::engine
