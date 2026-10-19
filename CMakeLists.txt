cmake_minimum_required(VERSION 3.16)
project(GeoCAD VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GEOCAD_BUILD_TESTS "Build unit tests" ON)

add_library(geocad_core
    src/geometry/Point2D.cpp
    src/geometry/Line2D.cpp
    src/geometry/Point3D.cpp
    src/geometry/Vector2D.cpp
    src/geometry/Vector3D.cpp
    src/geometry/Matrix3D.cpp
    src/geometry/BoundingBox3D.cpp
    src/shapes/Shape.cpp
    src/shapes/Circle.cpp
    src/shapes/Rectangle.cpp
    src/shapes/Triangle.cpp
    src/shapes/Polygon.cpp
    src/shapes/Sphere.cpp
    src/shapes/Cube.cpp
    src/shapes/Cylinder.cpp
    src/engine/GeometryEngine.cpp
    src/engine/IntersectionEngine.cpp
    src/engine/CollisionDetector.cpp
    src/scene/Scene.cpp
    src/scene/ShapeFactory.cpp
    src/commands/Command.cpp
    src/commands/CommandManager.cpp
    src/commands/ShapeCommands.cpp
    src/io/ModelSerializer.cpp
    src/io/ModelReader.cpp
    src/io/ModelWriter.cpp
    src/cli/CommandLineInterface.cpp
    src/utils/Logger.cpp
)

target_include_directories(geocad_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_options(geocad_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(geocad src/main.cpp)
target_link_libraries(geocad PRIVATE geocad_core)

if(GEOCAD_BUILD_TESTS)
    enable_testing()

    add_executable(geocad_tests
        tests/TestMain.cpp
        tests/GeometryTests.cpp
        tests/ShapeTests.cpp
        tests/IntersectionTests.cpp
        tests/CollisionTests.cpp
        tests/SceneTests.cpp
        tests/CommandTests.cpp
        tests/SerializationTests.cpp
    )
    target_link_libraries(geocad_tests PRIVATE geocad_core)
    target_include_directories(geocad_tests PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests)
    add_test(NAME geocad_tests COMMAND geocad_tests)
endif()
