#include "geocad/cli/CommandLineInterface.h"

#include "geocad/commands/ShapeCommands.h"
#include "geocad/engine/CollisionDetector.h"
#include "geocad/engine/GeometryEngine.h"
#include "geocad/engine/IntersectionEngine.h"
#include "geocad/geometry/Line2D.h"
#include "geocad/io/ModelReader.h"
#include "geocad/io/ModelWriter.h"
#include "geocad/scene/ShapeFactory.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace geocad::cli {

namespace {

double parseDouble(const std::string& token, const std::string& field)
{
    std::size_t consumed = 0;
    try {
        const double value = std::stod(token, &consumed);
        if (consumed != token.size()) {
            throw std::invalid_argument("unexpected characters");
        }
        return value;
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid " + field + ": '" + token + "'.");
    }
}

bool isBlank(const std::string& line)
{
    for (unsigned char character : line) {
        if (!std::isspace(character)) {
            return false;
        }
    }
    return true;
}

} // namespace

CommandLineInterface::CommandLineInterface(geocad::scene::Scene& scene, geocad::commands::CommandManager& commandManager)
    : scene_(scene), commandManager_(commandManager) {}

void CommandLineInterface::run(std::istream& input, std::ostream& output)
{
    output << "GeoCAD v1.0\n";
    output << "Type HELP for available commands.\n";

    std::string line;
    while (true) {
        output << "GeoCAD > ";
        if (!std::getline(input, line)) {
            break;
        }
        if (isBlank(line)) {
            continue;
        }
        try {
            if (!handleLine(line, output)) {
                break;
            }
        } catch (const std::exception& exception) {
            output << "ERROR: " << exception.what() << "\n";
        }
    }
}

std::vector<std::string> CommandLineInterface::tokenize(const std::string& line)
{
    std::istringstream input(line);
    std::vector<std::string> tokens;
    std::string token;
    while (input >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool CommandLineInterface::handleLine(const std::string& line, std::ostream& output)
{
    auto tokens = tokenize(line);
    if (tokens.empty()) {
        return true;
    }

    std::string command = tokens[0];
    for (char& character : command) {
        character = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    }

    if (command == "EXIT" || command == "QUIT") {
        output << "Goodbye.\n";
        return false;
    }

    if (command == "HELP") {
        printHelp(output);
        return true;
    }

    if (command == "LIST") {
        printList(output);
        return true;
    }

    if (command == "INFO") {
        if (tokens.size() != 2) throw std::invalid_argument("Usage: INFO <id>");
        printInfo(tokens[1], output);
        return true;
    }

    if (command == "CREATE") {
        if (tokens.size() < 3) throw std::invalid_argument("Usage: CREATE <TYPE> <arguments...>");
        std::vector<std::string> shapeTokens(tokens.begin() + 1, tokens.end());
        auto shape = geocad::scene::ShapeFactory::createFromTokens(shapeTokens);
        const std::string serialized = shape->serialize();
        std::string error;
        if (!commandManager_.execute(std::make_unique<geocad::commands::AddShapeCommand>(serialized), scene_, error)) {
            output << "ERROR: " << error << "\n";
        } else {
            output << "Created " << shape->typeName() << " '" << shape->id() << "'.\n";
        }
        return true;
    }

    if (command == "DELETE") {
        if (tokens.size() != 2) throw std::invalid_argument("Usage: DELETE <id>");
        std::string error;
        if (!commandManager_.execute(std::make_unique<geocad::commands::DeleteShapeCommand>(tokens[1]), scene_, error)) {
            output << "ERROR: " << error << "\n";
        } else {
            output << "Deleted '" << tokens[1] << "'.\n";
        }
        return true;
    }

    if (command == "MOVE") {
        if (tokens.size() != 4 && tokens.size() != 5) {
            throw std::invalid_argument("Usage: MOVE <id> <dx> <dy> [dz]");
        }
        const double dx = parseDouble(tokens[2], "dx");
        const double dy = parseDouble(tokens[3], "dy");
        const double dz = tokens.size() == 5 ? parseDouble(tokens[4], "dz") : 0.0;
        std::string error;
        if (!commandManager_.execute(std::make_unique<geocad::commands::MoveShapeCommand>(tokens[1], dx, dy, dz), scene_, error)) {
            output << "ERROR: " << error << "\n";
        } else {
            output << "Moved '" << tokens[1] << "'.\n";
        }
        return true;
    }

    if (command == "ROTATE") {
        if (tokens.size() != 3 && tokens.size() != 5) {
            throw std::invalid_argument("Usage: ROTATE <id> <rz> OR ROTATE <id> <rx> <ry> <rz>");
        }
        const double rx = tokens.size() == 3 ? 0.0 : parseDouble(tokens[2], "rx");
        const double ry = tokens.size() == 3 ? 0.0 : parseDouble(tokens[3], "ry");
        const double rz = tokens.size() == 3 ? parseDouble(tokens[2], "rz") : parseDouble(tokens[4], "rz");
        std::string error;
        if (!commandManager_.execute(std::make_unique<geocad::commands::RotateShapeCommand>(tokens[1], rx, ry, rz), scene_, error)) {
            output << "ERROR: " << error << "\n";
        } else {
            output << "Rotated '" << tokens[1] << "'.\n";
        }
        return true;
    }

    if (command == "SCALE") {
        if (tokens.size() != 3) throw std::invalid_argument("Usage: SCALE <id> <factor>");
        const double factor = parseDouble(tokens[2], "factor");
        std::string error;
        if (!commandManager_.execute(std::make_unique<geocad::commands::ScaleShapeCommand>(tokens[1], factor), scene_, error)) {
            output << "ERROR: " << error << "\n";
        } else {
            output << "Scaled '" << tokens[1] << "'.\n";
        }
        return true;
    }

    if (command == "DISTANCE") {
        if (tokens.size() != 3) throw std::invalid_argument("Usage: DISTANCE <id1> <id2>");
        const auto first = scene_.get(tokens[1]).referencePoint();
        const auto second = scene_.get(tokens[2]).referencePoint();
        output << std::fixed << std::setprecision(4)
               << "Reference-point distance: " << geocad::engine::GeometryEngine::distance(first, second) << "\n";
        return true;
    }

    if (command == "COLLISION") {
        if (tokens.size() != 3) throw std::invalid_argument("Usage: COLLISION <id1> <id2>");
        const bool collision = geocad::engine::CollisionDetector::collides(scene_.get(tokens[1]), scene_.get(tokens[2]));
        output << (collision ? "COLLISION DETECTED" : "NO COLLISION") << "\n";
        return true;
    }

    if (command == "INTERSECT") {
        if (tokens.size() != 10 || tokens[1] != "LINE") {
            throw std::invalid_argument("Usage: INTERSECT LINE x1 y1 x2 y2 x3 y3 x4 y4");
        }
        geocad::geometry::Line2D first(
            {parseDouble(tokens[2], "x1"), parseDouble(tokens[3], "y1")},
            {parseDouble(tokens[4], "x2"), parseDouble(tokens[5], "y2")});
        geocad::geometry::Line2D second(
            {parseDouble(tokens[6], "x3"), parseDouble(tokens[7], "y3")},
            {parseDouble(tokens[8], "x4"), parseDouble(tokens[9], "y4")});
        const auto result = geocad::engine::IntersectionEngine::intersectLines(first, second);
        switch (result.type) {
        case geocad::engine::LineIntersectionType::Point:
            output << std::fixed << std::setprecision(4)
                   << "Intersection: (" << result.point->x() << ", " << result.point->y() << ")\n";
            break;
        case geocad::engine::LineIntersectionType::Parallel:
            output << "Lines are parallel.\n";
            break;
        case geocad::engine::LineIntersectionType::Coincident:
            output << "Lines are coincident.\n";
            break;
        case geocad::engine::LineIntersectionType::None:
            output << "Line segments do not intersect.\n";
            break;
        }
        return true;
    }

    if (command == "SAVE") {
        if (tokens.size() != 2) throw std::invalid_argument("Usage: SAVE <file>");
        geocad::io::ModelWriter::writeToFile(scene_, tokens[1]);
        output << "Saved scene to '" << tokens[1] << "'.\n";
        return true;
    }

    if (command == "LOAD") {
        if (tokens.size() != 2) throw std::invalid_argument("Usage: LOAD <file>");
        geocad::scene::Scene loaded;
        geocad::io::ModelReader::readFromFile(tokens[1], loaded);
        scene_ = std::move(loaded);
        commandManager_.clear();
        output << "Loaded scene from '" << tokens[1] << "'.\n";
        return true;
    }

    if (command == "CLEAR") {
        scene_.clear();
        commandManager_.clear();
        output << "Scene cleared.\n";
        return true;
    }

    if (command == "UNDO") {
        std::string message;
        commandManager_.undo(scene_, message);
        output << message << "\n";
        return true;
    }

    if (command == "REDO") {
        std::string message;
        commandManager_.redo(scene_, message);
        output << message << "\n";
        return true;
    }

    if (command == "STATS") {
        output << "Objects: " << scene_.size() << " | 2D: " << scene_.count2D() << " | 3D: " << scene_.count3D() << "\n";
        return true;
    }

    throw std::invalid_argument("Unknown command: " + command + ". Type HELP for help.");
}

void CommandLineInterface::printHelp(std::ostream& output) const
{
    output << R"HELP(
Commands:
  CREATE CIRCLE <id> <x> <y> <radius>
  CREATE RECTANGLE <id> <cx> <cy> <width> <height> [rotation]
  CREATE TRIANGLE <id> <x1> <y1> <x2> <y2> <x3> <y3>
  CREATE POLYGON <id> <n> <x1> <y1> ... <xn> <yn>
  CREATE SPHERE <id> <x> <y> <z> <radius>
  CREATE CUBE <id> <x> <y> <z> <side> [rx ry rz]
  CREATE CYLINDER <id> <x> <y> <z> <radius> <height> [rx ry rz]
  DELETE <id>
  LIST
  INFO <id>
  MOVE <id> <dx> <dy> [dz]
  ROTATE <id> <rz>
  ROTATE <id> <rx> <ry> <rz>
  SCALE <id> <factor>
  DISTANCE <id1> <id2>
  COLLISION <id1> <id2>
  INTERSECT LINE <x1> <y1> <x2> <y2> <x3> <y3> <x4> <y4>
  SAVE <file>
  LOAD <file>
  UNDO
  REDO
  STATS
  CLEAR
  HELP
  EXIT
)HELP";
}

void CommandLineInterface::printInfo(const std::string& id, std::ostream& output) const
{
    const auto& shape = scene_.get(id);
    const auto center = shape.referencePoint();
    const auto box = shape.boundingBox();

    output << "Type: " << shape.typeName() << "\n";
    output << "Dimension: " << (shape.dimension() == geocad::shapes::ShapeDimension::TwoD ? "2D" : "3D") << "\n";
    output << std::fixed << std::setprecision(4);
    output << "Reference point: (" << center.x() << ", " << center.y() << ", " << center.z() << ")\n";
    output << "Area / surface area: " << shape.area() << "\n";
    output << "Perimeter: " << shape.perimeter() << "\n";
    output << "Volume: " << shape.volume() << "\n";
    output << "AABB min: (" << box.minimum().x() << ", " << box.minimum().y() << ", " << box.minimum().z() << ")\n";
    output << "AABB max: (" << box.maximum().x() << ", " << box.maximum().y() << ", " << box.maximum().z() << ")\n";
}

void CommandLineInterface::printList(std::ostream& output) const
{
    const auto ids = scene_.listIds();
    if (ids.empty()) {
        output << "Scene is empty.\n";
        return;
    }
    output << "ID\tTYPE\tDIMENSION\n";
    for (const auto& id : ids) {
        const auto& shape = scene_.get(id);
        output << id << '\t' << shape.typeName() << '\t'
               << (shape.dimension() == geocad::shapes::ShapeDimension::TwoD ? "2D" : "3D") << '\n';
    }
}

} // namespace geocad::cli
