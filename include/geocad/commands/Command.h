#pragma once

#include <string>

namespace geocad::scene { class Scene; }

namespace geocad::commands {

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(geocad::scene::Scene& scene) = 0;
    virtual void undo(geocad::scene::Scene& scene) = 0;
    virtual std::string description() const = 0;
};

} // namespace geocad::commands
