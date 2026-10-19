#include "TestFramework.h"

#include "geocad/io/ModelReader.h"
#include "geocad/io/ModelWriter.h"
#include "geocad/scene/Scene.h"
#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Cube.h"

#include <filesystem>

void runSerializationTests()
{
    geocad_test::run("save and load", [] {
        const auto path = std::filesystem::temp_directory_path() / "geocad_test_scene.geocad";
        geocad::scene::Scene original;
        original.add(std::make_unique<geocad::shapes::Circle>("c1", 10.0, 20.0, 5.0));
        original.add(std::make_unique<geocad::shapes::Cube>("cube1", 1.0, 2.0, 3.0, 4.0, 10.0, 20.0, 30.0));
        geocad::io::ModelWriter::writeToFile(original, path.string());
        geocad::scene::Scene loaded;
        geocad::io::ModelReader::readFromFile(path.string(), loaded);
        EXPECT_EQ(original.size(), loaded.size());
        EXPECT_EQ(original.serialize(), loaded.serialize());
        std::error_code error;
        std::filesystem::remove(path, error);
    });

    geocad_test::run("invalid state is rejected", [] {
        geocad::scene::Scene scene;
        EXPECT_THROW(scene.restoreFromSerialized("NOT_GEOCAD\n"));
    });
}
