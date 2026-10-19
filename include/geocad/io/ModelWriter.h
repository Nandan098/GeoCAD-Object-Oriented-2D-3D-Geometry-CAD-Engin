#pragma once

#include "geocad/scene/Scene.h"

#include <string>

namespace geocad::io {

class ModelWriter {
public:
    static void writeToFile(const geocad::scene::Scene& scene, const std::string& path);
};

} // namespace geocad::io
