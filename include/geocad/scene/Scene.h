#pragma once

#include "geocad/shapes/Shape.h"

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace geocad::scene {

class Scene {
public:
    Scene() = default;

    bool contains(const std::string& id) const;
    void add(std::unique_ptr<geocad::shapes::Shape> shape);
    void addFromSerialized(const std::string& line);
    std::unique_ptr<geocad::shapes::Shape> remove(const std::string& id);

    geocad::shapes::Shape& get(const std::string& id);
    const geocad::shapes::Shape& get(const std::string& id) const;

    std::vector<std::string> listIds() const;
    std::size_t size() const;
    std::size_t count2D() const;
    std::size_t count3D() const;

    void clear();
    void restoreFromSerialized(const std::string& state);
    std::string serialize() const;

private:
    std::unordered_map<std::string, std::unique_ptr<geocad::shapes::Shape>> objects_;
};

} // namespace geocad::scene
