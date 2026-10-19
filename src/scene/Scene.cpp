#include "geocad/scene/Scene.h"
#include "geocad/scene/ShapeFactory.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace geocad::scene {

bool Scene::contains(const std::string& id) const
{
    return objects_.find(id) != objects_.end();
}

void Scene::add(std::unique_ptr<geocad::shapes::Shape> shape)
{
    if (!shape) {
        throw std::invalid_argument("Cannot add a null shape.");
    }
    if (shape->id().empty()) {
        throw std::invalid_argument("Shape ID cannot be empty.");
    }
    if (contains(shape->id())) {
        throw std::invalid_argument("An object with ID '" + shape->id() + "' already exists.");
    }
    objects_.emplace(shape->id(), std::move(shape));
}

void Scene::addFromSerialized(const std::string& line)
{
    add(ShapeFactory::createFromSerialized(line));
}

std::unique_ptr<geocad::shapes::Shape> Scene::remove(const std::string& id)
{
    const auto it = objects_.find(id);
    if (it == objects_.end()) {
        throw std::out_of_range("Object '" + id + "' does not exist.");
    }

    auto result = std::move(it->second);
    objects_.erase(it);
    return result;
}

geocad::shapes::Shape& Scene::get(const std::string& id)
{
    const auto it = objects_.find(id);
    if (it == objects_.end()) {
        throw std::out_of_range("Object '" + id + "' does not exist.");
    }
    return *it->second;
}

const geocad::shapes::Shape& Scene::get(const std::string& id) const
{
    const auto it = objects_.find(id);
    if (it == objects_.end()) {
        throw std::out_of_range("Object '" + id + "' does not exist.");
    }
    return *it->second;
}

std::vector<std::string> Scene::listIds() const
{
    std::vector<std::string> ids;
    ids.reserve(objects_.size());
    for (const auto& [id, shape] : objects_) {
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

std::size_t Scene::size() const { return objects_.size(); }

std::size_t Scene::count2D() const
{
    std::size_t count = 0;
    for (const auto& [id, shape] : objects_) {
        if (shape->dimension() == geocad::shapes::ShapeDimension::TwoD) {
            ++count;
        }
    }
    return count;
}

std::size_t Scene::count3D() const
{
    return size() - count2D();
}

void Scene::clear()
{
    objects_.clear();
}

std::string Scene::serialize() const
{
    std::ostringstream output;
    output << "GEOCAD 1\n";
    output << "SHAPES " << objects_.size() << "\n";
    for (const auto& id : listIds()) {
        output << objects_.at(id)->serialize() << "\n";
    }
    output << "END\n";
    return output.str();
}

void Scene::restoreFromSerialized(const std::string& state)
{
    std::istringstream input(state);
    std::string line;
    std::vector<std::string> shapeLines;

    if (!std::getline(input, line) || line != "GEOCAD 1") {
        throw std::runtime_error("Invalid GeoCAD state header.");
    }

    if (!std::getline(input, line) || line.rfind("SHAPES ", 0) != 0) {
        throw std::runtime_error("Invalid GeoCAD shape-count line.");
    }

    const std::size_t expectedCount = static_cast<std::size_t>(std::stoull(line.substr(7)));

    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }
        if (line == "END") {
            break;
        }
        shapeLines.push_back(line);
    }

    if (shapeLines.size() != expectedCount) {
        throw std::runtime_error("GeoCAD state shape count does not match its contents.");
    }

    std::unordered_map<std::string, std::unique_ptr<geocad::shapes::Shape>> newObjects;
    for (const auto& shapeLine : shapeLines) {
        auto shape = ShapeFactory::createFromSerialized(shapeLine);
        if (!newObjects.emplace(shape->id(), std::move(shape)).second) {
            throw std::runtime_error("Duplicate object ID in GeoCAD state.");
        }
    }

    objects_ = std::move(newObjects);
}

} // namespace geocad::scene
