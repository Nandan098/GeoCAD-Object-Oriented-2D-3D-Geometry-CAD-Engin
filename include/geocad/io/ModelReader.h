#pragma once

#include "geocad/scene/Scene.h"

#include <string>

namespace geocad::io {

class ModelReader {
public:
    static void readFromFile(const std::string& path, geocad::scene::Scene& scene);
};

} // namespace geocad::io
