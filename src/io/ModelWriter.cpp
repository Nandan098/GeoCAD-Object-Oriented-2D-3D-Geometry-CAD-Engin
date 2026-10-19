#include "geocad/io/ModelWriter.h"
#include "geocad/io/ModelSerializer.h"

#include <fstream>
#include <stdexcept>

namespace geocad::io {

void ModelWriter::writeToFile(const geocad::scene::Scene& scene, const std::string& path)
{
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Could not open file for writing: " + path);
    }

    output << ModelSerializer::serialize(scene);
    if (!output) {
        throw std::runtime_error("Failed while writing file: " + path);
    }
}

} // namespace geocad::io
