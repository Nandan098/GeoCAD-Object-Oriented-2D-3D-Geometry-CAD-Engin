#include "geocad/commands/CommandManager.h"
#include "geocad/scene/Scene.h"

#include <utility>

namespace geocad::commands {

bool CommandManager::execute(std::unique_ptr<Command> command, geocad::scene::Scene& scene, std::string& error)
{
    if (!command) {
        error = "Cannot execute a null command.";
        return false;
    }

    try {
        command->execute(scene);
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }

    undoStack_.push(std::move(command));
    while (!redoStack_.empty()) {
        redoStack_.pop();
    }
    return true;
}

bool CommandManager::undo(geocad::scene::Scene& scene, std::string& message)
{
    if (undoStack_.empty()) {
        message = "Nothing to undo.";
        return false;
    }

    auto command = std::move(undoStack_.top());
    undoStack_.pop();
    try {
        command->undo(scene);
    } catch (const std::exception& exception) {
        message = exception.what();
        undoStack_.push(std::move(command));
        return false;
    }
    message = "Undid: " + command->description();
    redoStack_.push(std::move(command));
    return true;
}

bool CommandManager::redo(geocad::scene::Scene& scene, std::string& message)
{
    if (redoStack_.empty()) {
        message = "Nothing to redo.";
        return false;
    }

    auto command = std::move(redoStack_.top());
    redoStack_.pop();
    try {
        command->execute(scene);
    } catch (const std::exception& exception) {
        message = exception.what();
        redoStack_.push(std::move(command));
        return false;
    }
    message = "Redid: " + command->description();
    undoStack_.push(std::move(command));
    return true;
}

bool CommandManager::canUndo() const { return !undoStack_.empty(); }
bool CommandManager::canRedo() const { return !redoStack_.empty(); }

void CommandManager::clear()
{
    while (!undoStack_.empty()) undoStack_.pop();
    while (!redoStack_.empty()) redoStack_.pop();
}

} // namespace geocad::commands
