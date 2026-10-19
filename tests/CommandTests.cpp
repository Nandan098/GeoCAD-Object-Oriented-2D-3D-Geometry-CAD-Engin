#include "TestFramework.h"

#include "geocad/commands/CommandManager.h"
#include "geocad/commands/ShapeCommands.h"
#include "geocad/scene/Scene.h"
#include "geocad/shapes/Circle.h"

void runCommandTests()
{
    geocad_test::run("add undo redo", [] {
        geocad::scene::Scene scene;
        geocad::commands::CommandManager manager;
        std::string error;
        EXPECT_TRUE(manager.execute(std::make_unique<geocad::commands::AddShapeCommand>("CIRCLE c1 0 0 5"), scene, error));
        EXPECT_TRUE(scene.contains("c1"));
        std::string message;
        EXPECT_TRUE(manager.undo(scene, message));
        EXPECT_FALSE(scene.contains("c1"));
        EXPECT_TRUE(manager.redo(scene, message));
        EXPECT_TRUE(scene.contains("c1"));
    });

    geocad_test::run("move undo", [] {
        geocad::scene::Scene scene;
        geocad::commands::CommandManager manager;
        std::string error;
        EXPECT_TRUE(manager.execute(std::make_unique<geocad::commands::AddShapeCommand>("CIRCLE c1 1 2 3"), scene, error));
        EXPECT_TRUE(manager.execute(std::make_unique<geocad::commands::MoveShapeCommand>("c1", 10, 20, 0), scene, error));
        const auto& circle = static_cast<const geocad::shapes::Circle&>(scene.get("c1"));
        EXPECT_NEAR(11.0, circle.centerX(), 1e-12);
        EXPECT_NEAR(22.0, circle.centerY(), 1e-12);
        std::string message;
        EXPECT_TRUE(manager.undo(scene, message));
        const auto& restored = static_cast<const geocad::shapes::Circle&>(scene.get("c1"));
        EXPECT_NEAR(1.0, restored.centerX(), 1e-12);
        EXPECT_NEAR(2.0, restored.centerY(), 1e-12);
    });

    geocad_test::run("new command clears redo", [] {
        geocad::scene::Scene scene;
        geocad::commands::CommandManager manager;
        std::string error;
        EXPECT_TRUE(manager.execute(std::make_unique<geocad::commands::AddShapeCommand>("CIRCLE a 0 0 1"), scene, error));
        std::string message;
        EXPECT_TRUE(manager.undo(scene, message));
        EXPECT_TRUE(manager.canRedo());
        EXPECT_TRUE(manager.execute(std::make_unique<geocad::commands::AddShapeCommand>("CIRCLE b 0 0 1"), scene, error));
        EXPECT_FALSE(manager.canRedo());
    });
}
