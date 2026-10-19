#include "geocad/io/ModelSerializer.h"

namespace geocad::io {

std::string ModelSerializer::serialize(const geocad::scene::Scene& scene)
{
    return scene.serialize();
}

void ModelSerializer::deserialize(const std::string& data, geocad::scene::Scene& scene)
{
    scene.restoreFromSerialized(data);
}

} // namespace geocad::io
