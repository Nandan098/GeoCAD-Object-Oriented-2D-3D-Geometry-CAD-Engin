#pragma once

#include "geocad/geometry/Line2D.h"
#include "geocad/geometry/Point2D.h"
#include "geocad/shapes/Polygon.h"
#include "geocad/shapes/Rectangle.h"
#include "geocad/shapes/Circle.h"

#include <optional>
#include <vector>

namespace geocad::engine {

enum class LineIntersectionType {
    None,
    Point,
    Parallel,
    Coincident
};

struct LineIntersectionResult {
    LineIntersectionType type{LineIntersectionType::None};
    std::optional<geocad::geometry::Point2D> point;
};

class IntersectionEngine {
public:
    static LineIntersectionResult intersectLines(const geocad::geometry::Line2D& first, const geocad::geometry::Line2D& second);
    static std::vector<geocad::geometry::Point2D> intersectLineCircle(const geocad::geometry::Line2D& line, const geocad::shapes::Circle& circle);
    static std::vector<geocad::geometry::Point2D> intersectCircles(const geocad::shapes::Circle& first, const geocad::shapes::Circle& second);
    static bool pointInPolygon(const geocad::geometry::Point2D& point, const geocad::shapes::Polygon& polygon);
};

} // namespace geocad::engine
