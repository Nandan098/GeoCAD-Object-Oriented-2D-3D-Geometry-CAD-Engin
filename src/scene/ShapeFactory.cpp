#include "geocad/scene/ShapeFactory.h"

#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Cube.h"
#include "geocad/shapes/Cylinder.h"
#include "geocad/shapes/Polygon.h"
#include "geocad/shapes/Rectangle.h"
#include "geocad/shapes/Sphere.h"
#include "geocad/shapes/Triangle.h"

#include <sstream>
#include <stdexcept>

namespace geocad::scene {

namespace {

double parseDouble(const std::string& token, const std::string& field)
{
    try {
        std::size_t consumed = 0;
        const double value = std::stod(token, &consumed);
        if (consumed != token.size()) {
            throw std::invalid_argument("not all characters were consumed");
        }
        return value;
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid numeric value for " + field + ": '" + token + "'.");
    }
}

std::size_t parseSize(const std::string& token, const std::string& field)
{
    try {
        std::size_t consumed = 0;
        const auto value = std::stoull(token, &consumed);
        if (consumed != token.size()) {
            throw std::invalid_argument("not all characters were consumed");
        }
        return static_cast<std::size_t>(value);
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid integer value for " + field + ": '" + token + "'.");
    }
}

} // namespace

std::unique_ptr<geocad::shapes::Shape> ShapeFactory::createFromTokens(const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2) {
        throw std::invalid_argument("A shape command needs a type and an object ID.");
    }

    const std::string& type = tokens[0];
    const std::string& id = tokens[1];
    if (id.empty()) {
        throw std::invalid_argument("Object ID cannot be empty.");
    }

    if (type == "CIRCLE") {
        if (tokens.size() != 5) throw std::invalid_argument("CREATE CIRCLE requires: id x y radius.");
        return std::make_unique<geocad::shapes::Circle>(id, parseDouble(tokens[2], "x"), parseDouble(tokens[3], "y"), parseDouble(tokens[4], "radius"));
    }

    if (type == "RECTANGLE") {
        if (tokens.size() != 6 && tokens.size() != 7) throw std::invalid_argument("CREATE RECTANGLE requires: id cx cy width height [rotation].");
        const double rotation = tokens.size() == 7 ? parseDouble(tokens[6], "rotation") : 0.0;
        return std::make_unique<geocad::shapes::Rectangle>(id, parseDouble(tokens[2], "centerX"), parseDouble(tokens[3], "centerY"), parseDouble(tokens[4], "width"), parseDouble(tokens[5], "height"), rotation);
    }

    if (type == "TRIANGLE") {
        if (tokens.size() != 8) throw std::invalid_argument("CREATE TRIANGLE requires: id x1 y1 x2 y2 x3 y3.");
        return std::make_unique<geocad::shapes::Triangle>(
            id,
            geocad::geometry::Point2D(parseDouble(tokens[2], "x1"), parseDouble(tokens[3], "y1")),
            geocad::geometry::Point2D(parseDouble(tokens[4], "x2"), parseDouble(tokens[5], "y2")),
            geocad::geometry::Point2D(parseDouble(tokens[6], "x3"), parseDouble(tokens[7], "y3")));
    }

    if (type == "POLYGON") {
        if (tokens.size() < 3) throw std::invalid_argument("CREATE POLYGON requires: id count coordinates.");
        const std::size_t count = parseSize(tokens[2], "vertex count");
        if (count < 3 || tokens.size() != 3 + 2 * count) {
            throw std::invalid_argument("Polygon vertex count does not match the number of coordinates.");
        }
        std::vector<geocad::geometry::Point2D> vertices;
        vertices.reserve(count);
        for (std::size_t i = 0; i < count; ++i) {
            vertices.emplace_back(
                parseDouble(tokens[3 + 2 * i], "x"),
                parseDouble(tokens[4 + 2 * i], "y"));
        }
        return std::make_unique<geocad::shapes::Polygon>(id, vertices);
    }

    if (type == "SPHERE") {
        if (tokens.size() != 6) throw std::invalid_argument("CREATE SPHERE requires: id x y z radius.");
        return std::make_unique<geocad::shapes::Sphere>(id, parseDouble(tokens[2], "x"), parseDouble(tokens[3], "y"), parseDouble(tokens[4], "z"), parseDouble(tokens[5], "radius"));
    }

    if (type == "CUBE") {
        if (tokens.size() != 6 && tokens.size() != 9) throw std::invalid_argument("CREATE CUBE requires: id x y z side [rx ry rz].");
        const double rx = tokens.size() == 9 ? parseDouble(tokens[6], "rx") : 0.0;
        const double ry = tokens.size() == 9 ? parseDouble(tokens[7], "ry") : 0.0;
        const double rz = tokens.size() == 9 ? parseDouble(tokens[8], "rz") : 0.0;
        return std::make_unique<geocad::shapes::Cube>(id, parseDouble(tokens[2], "x"), parseDouble(tokens[3], "y"), parseDouble(tokens[4], "z"), parseDouble(tokens[5], "side"), rx, ry, rz);
    }

    if (type == "CYLINDER") {
        if (tokens.size() != 7 && tokens.size() != 10) throw std::invalid_argument("CREATE CYLINDER requires: id x y z radius height [rx ry rz].");
        const double rx = tokens.size() == 10 ? parseDouble(tokens[7], "rx") : 0.0;
        const double ry = tokens.size() == 10 ? parseDouble(tokens[8], "ry") : 0.0;
        const double rz = tokens.size() == 10 ? parseDouble(tokens[9], "rz") : 0.0;
        return std::make_unique<geocad::shapes::Cylinder>(id, parseDouble(tokens[2], "x"), parseDouble(tokens[3], "y"), parseDouble(tokens[4], "z"), parseDouble(tokens[5], "radius"), parseDouble(tokens[6], "height"), rx, ry, rz);
    }

    throw std::invalid_argument("Unsupported shape type: " + type);
}

std::unique_ptr<geocad::shapes::Shape> ShapeFactory::createFromSerialized(const std::string& line)
{
    std::istringstream input(line);
    std::vector<std::string> tokens;
    std::string token;
    while (input >> token) {
        tokens.push_back(token);
    }
    return createFromTokens(tokens);
}

} // namespace geocad::scene
