#pragma once

#include "geocad/geometry/Point2D.h"

namespace geocad::geometry {

class Line2D {
public:
    Line2D(const Point2D& start, const Point2D& end);

    const Point2D& start() const;
    const Point2D& end() const;

private:
    Point2D start_;
    Point2D end_;
};

} // namespace geocad::geometry
