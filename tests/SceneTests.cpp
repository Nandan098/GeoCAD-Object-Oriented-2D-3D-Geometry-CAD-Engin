#include "TestFramework.h"

#include "geocad/scene/Scene.h"
#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Cube.h"

void runSceneTests()
{
    geocad_test::run("scene add/find/remove", [] {
        geocad::scene::Scene scene;
        scene.add(std::make_unique<geocad::shapes::Circle>("c1", 0.0, 0.0, 2.0));
        scene.add(std::make_unique<geocad::shapes::Cube>("cube1", 0.0, 0.0, 0.0, 1.0));
        EXPECT_TRUE(scene.contains("c1"));
        EXPECT_EQ(static_cast<std::size_t>(2), scene.size());
        EXPECT_EQ(static_cast<std::size_t>(1), scene.count2D());
        EXPECT_EQ(static_cast<std::size_t>(1), scene.count3D());
        scene.remove("c1");
        EXPECT_FALSE(scene.contains("c1"));
    });

    geocad_test::run("duplicate IDs are rejected", [] {
        geocad::scene::Scene scene;
        scene.add(std::make_unique<geocad::shapes::Circle>("c1", 0.0, 0.0, 2.0));
        EXPECT_THROW(scene.add(std::make_unique<geocad::shapes::Circle>("c1", 1.0, 1.0, 3.0)));
    });

    geocad_test::run("deterministic scene serialization", [] {
        geocad::scene::Scene scene;
        scene.add(std::make_unique<geocad::shapes::Circle>("z", 0.0, 0.0, 1.0));
        scene.add(std::make_unique<geocad::shapes::Circle>("a", 1.0, 2.0, 3.0));
        const std::string data = scene.serialize();
        const auto firstA = data.find("CIRCLE a");
        const auto firstZ = data.find("CIRCLE z");
        EXPECT_NE(std::string::npos, firstA);
        EXPECT_NE(std::string::npos, firstZ);
        EXPECT_TRUE(firstA < firstZ);
    });
}
