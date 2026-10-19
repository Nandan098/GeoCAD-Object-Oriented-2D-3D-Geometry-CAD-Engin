#pragma once

#include "geocad/shapes/Shape.h"

#include <memory>
#include <string>
#include <vector>

namespace geocad::scene {

class ShapeFactory {
public:
    static std::unique_ptr<geocad::shapes::Shape> createFromTokens(const std::vector<std::string>& tokens);
    static std::unique_ptr<geocad::shapes::Shape> createFromSerialized(const std::string& line);
};

} // namespace geocad::scene
