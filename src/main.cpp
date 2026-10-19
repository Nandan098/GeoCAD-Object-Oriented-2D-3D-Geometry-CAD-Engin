#include "geocad/cli/CommandLineInterface.h"
#include "geocad/commands/CommandManager.h"
#include "geocad/scene/Scene.h"

#include <iostream>

int main()
{
    geocad::scene::Scene scene;
    geocad::commands::CommandManager commandManager;
    geocad::cli::CommandLineInterface cli(scene, commandManager);
    cli.run(std::cin, std::cout);
    return 0;
}
