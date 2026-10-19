#include "geocad/commands/ShapeCommands.h"
#include "geocad/scene/Scene.h"

#include <stdexcept>
#include <utility>

namespace geocad::commands {

void StatefulCommand::execute(geocad::scene::Scene& scene)
{
    if (!initialized_) {
        before_ = scene.serialize();
        apply(scene);
        after_ = scene.serialize();
        initialized_ = true;
        return;
    }

    scene.restoreFromSerialized(after_);
}

void StatefulCommand::undo(geocad::scene::Scene& scene)
{
    if (!initialized_) {
        throw std::logic_error("Cannot undo a command that has not been executed.");
    }
    scene.restoreFromSerialized(before_);
}

AddShapeCommand::AddShapeCommand(std::string serializedShape)
    : serializedShape_(std::move(serializedShape)) {}

std::string AddShapeCommand::description() const { return "Add shape"; }

void AddShapeCommand::apply(geocad::scene::Scene& scene)
{
    scene.addFromSerialized(serializedShape_);
}

DeleteShapeCommand::DeleteShapeCommand(std::string id) : id_(std::move(id)) {}

std::string DeleteShapeCommand::description() const { return "Delete shape '" + id_ + "'"; }

void DeleteShapeCommand::apply(geocad::scene::Scene& scene)
{
    scene.remove(id_);
}

MoveShapeCommand::MoveShapeCommand(std::string id, double dx, double dy, double dz)
    : id_(std::move(id)), dx_(dx), dy_(dy), dz_(dz) {}

std::string MoveShapeCommand::description() const { return "Move shape '" + id_ + "'"; }

void MoveShapeCommand::apply(geocad::scene::Scene& scene)
{
    scene.get(id_).translate(geocad::geometry::Vector3D(dx_, dy_, dz_));
}

RotateShapeCommand::RotateShapeCommand(std::string id, double rx, double ry, double rz)
    : id_(std::move(id)), rx_(rx), ry_(ry), rz_(rz) {}

std::string RotateShapeCommand::description() const { return "Rotate shape '" + id_ + "'"; }

void RotateShapeCommand::apply(geocad::scene::Scene& scene)
{
    scene.get(id_).rotate(rx_, ry_, rz_);
}

ScaleShapeCommand::ScaleShapeCommand(std::string id, double factor)
    : id_(std::move(id)), factor_(factor) {}

std::string ScaleShapeCommand::description() const { return "Scale shape '" + id_ + "'"; }

void ScaleShapeCommand::apply(geocad::scene::Scene& scene)
{
    scene.get(id_).scale(factor_);
}

} // namespace geocad::commands
