#include "geocad/geometry/Line2D.h"

namespace geocad::geometry {

Line2D::Line2D(const Point2D& start, const Point2D& end) : start_(start), end_(end) {}

const Point2D& Line2D::start() const { return start_; }
const Point2D& Line2D::end() const { return end_; }

} // namespace geocad::geometry
