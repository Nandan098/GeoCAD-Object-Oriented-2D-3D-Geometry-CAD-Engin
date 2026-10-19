#include "geocad/io/ModelReader.h"
#include "geocad/io/ModelSerializer.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace geocad::io {

void ModelReader::readFromFile(const std::string& path, geocad::scene::Scene& scene)
{
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Could not open file for reading: " + path);
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    if (!input.good() && !input.eof()) {
        throw std::runtime_error("Failed while reading file: " + path);
    }

    ModelSerializer::deserialize(buffer.str(), scene);
}

} // namespace geocad::io
