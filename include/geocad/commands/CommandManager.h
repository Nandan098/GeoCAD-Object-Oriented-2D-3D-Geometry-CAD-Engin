#pragma once

#include "geocad/commands/Command.h"

#include <memory>
#include <stack>

namespace geocad::commands {

class CommandManager {
public:
    bool execute(std::unique_ptr<Command> command, geocad::scene::Scene& scene, std::string& error);
    bool undo(geocad::scene::Scene& scene, std::string& message);
    bool redo(geocad::scene::Scene& scene, std::string& message);

    bool canUndo() const;
    bool canRedo() const;
    void clear();

private:
    std::stack<std::unique_ptr<Command>> undoStack_;
    std::stack<std::unique_ptr<Command>> redoStack_;
};

} // namespace geocad::commands
