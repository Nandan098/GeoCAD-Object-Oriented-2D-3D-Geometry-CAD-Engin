#pragma once

#include "geocad/commands/CommandManager.h"
#include "geocad/scene/Scene.h"

#include <iosfwd>
#include <string>
#include <vector>

namespace geocad::cli {

class CommandLineInterface {
public:
    CommandLineInterface(geocad::scene::Scene& scene, geocad::commands::CommandManager& commandManager);

    void run(std::istream& input, std::ostream& output);

private:
    bool handleLine(const std::string& line, std::ostream& output);
    static std::vector<std::string> tokenize(const std::string& line);
    void printHelp(std::ostream& output) const;
    void printInfo(const std::string& id, std::ostream& output) const;
    void printList(std::ostream& output) const;

    geocad::scene::Scene& scene_;
    geocad::commands::CommandManager& commandManager_;
};

} // namespace geocad::cli
