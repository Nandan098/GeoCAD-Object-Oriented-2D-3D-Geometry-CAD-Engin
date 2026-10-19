#include "geocad/shapes/Shape.h"

#include <utility>

namespace geocad::shapes {

Shape::Shape(std::string id) : id_(std::move(id)) {}

const std::string& Shape::id() const { return id_; }

} // namespace geocad::shapes
