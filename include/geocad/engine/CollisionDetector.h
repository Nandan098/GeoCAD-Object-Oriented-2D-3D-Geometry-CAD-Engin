#pragma once

#include "geocad/shapes/Shape.h"

namespace geocad::engine {

class CollisionDetector {
public:
    static bool collides(const geocad::shapes::Shape& first, const geocad::shapes::Shape& second);

private:
    static bool circleCircle(const geocad::shapes::Shape& first, const geocad::shapes::Shape& second);
    static bool circleRectangle(const geocad::shapes::Shape& circle, const geocad::shapes::Shape& rectangle);
    static bool rectangleRectangle(const geocad::shapes::Shape& first, const geocad::shapes::Shape& second);
};

} // namespace geocad::engine
