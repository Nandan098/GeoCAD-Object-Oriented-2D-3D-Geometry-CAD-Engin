#pragma once

#include "geocad/scene/Scene.h"

#include <string>

namespace geocad::io {

class ModelSerializer {
public:
    static std::string serialize(const geocad::scene::Scene& scene);
    static void deserialize(const std::string& data, geocad::scene::Scene& scene);
};

} // namespace geocad::io
