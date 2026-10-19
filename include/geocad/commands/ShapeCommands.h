#pragma once

#include "geocad/commands/Command.h"

#include <memory>
#include <string>

namespace geocad::commands {

class StatefulCommand : public Command {
public:
    void execute(geocad::scene::Scene& scene) override;
    void undo(geocad::scene::Scene& scene) override;

protected:
    virtual void apply(geocad::scene::Scene& scene) = 0;
    std::string before_;
    std::string after_;
    bool initialized_{false};
};

class AddShapeCommand final : public StatefulCommand {
public:
    explicit AddShapeCommand(std::string serializedShape);
    std::string description() const override;

protected:
    void apply(geocad::scene::Scene& scene) override;

private:
    std::string serializedShape_;
};

class DeleteShapeCommand final : public StatefulCommand {
public:
    explicit DeleteShapeCommand(std::string id);
    std::string description() const override;

protected:
    void apply(geocad::scene::Scene& scene) override;

private:
    std::string id_;
};

class MoveShapeCommand final : public StatefulCommand {
public:
    MoveShapeCommand(std::string id, double dx, double dy, double dz = 0.0);
    std::string description() const override;

protected:
    void apply(geocad::scene::Scene& scene) override;

private:
    std::string id_;
    double dx_{0.0};
    double dy_{0.0};
    double dz_{0.0};
};

class RotateShapeCommand final : public StatefulCommand {
public:
    RotateShapeCommand(std::string id, double rx, double ry, double rz);
    std::string description() const override;

protected:
    void apply(geocad::scene::Scene& scene) override;

private:
    std::string id_;
    double rx_{0.0};
    double ry_{0.0};
    double rz_{0.0};
};

class ScaleShapeCommand final : public StatefulCommand {
public:
    ScaleShapeCommand(std::string id, double factor);
    std::string description() const override;

protected:
    void apply(geocad::scene::Scene& scene) override;

private:
    std::string id_;
    double factor_{1.0};
};

} // namespace geocad::commands
