#include "TestFramework.h"

#include "geocad/engine/GeometryEngine.h"
#include "geocad/geometry/Line2D.h"
#include "geocad/geometry/Vector2D.h"
#include "geocad/geometry/Vector3D.h"

void runGeometryTests()
{
    geocad_test::run("2D distance", [] {
        geocad::geometry::Point2D a(0.0, 0.0);
        geocad::geometry::Point2D b(3.0, 4.0);
        EXPECT_NEAR(5.0, geocad::engine::GeometryEngine::distance(a, b), 1e-12);
    });

    geocad_test::run("3D distance", [] {
        geocad::geometry::Point3D a(0.0, 0.0, 0.0);
        geocad::geometry::Point3D b(1.0, 2.0, 2.0);
        EXPECT_NEAR(3.0, geocad::engine::GeometryEngine::distance(a, b), 1e-12);
    });

    geocad_test::run("2D dot/cross", [] {
        geocad::geometry::Vector2D a(1.0, 2.0);
        geocad::geometry::Vector2D b(3.0, 4.0);
        EXPECT_NEAR(11.0, a.dot(b), 1e-12);
        EXPECT_NEAR(-2.0, a.cross(b), 1e-12);
    });

    geocad_test::run("3D cross", [] {
        geocad::geometry::Vector3D x(1.0, 0.0, 0.0);
        geocad::geometry::Vector3D y(0.0, 1.0, 0.0);
        const auto z = x.cross(y);
        EXPECT_NEAR(0.0, z.x(), 1e-12);
        EXPECT_NEAR(0.0, z.y(), 1e-12);
        EXPECT_NEAR(1.0, z.z(), 1e-12);
    });

    geocad_test::run("point to line distance", [] {
        geocad::geometry::Line2D line({0.0, 0.0}, {10.0, 0.0});
        geocad::geometry::Point2D point(5.0, 3.0);
        EXPECT_NEAR(3.0, geocad::engine::GeometryEngine::pointToLineDistance(point, line), 1e-12);
    });

    geocad_test::run("angle", [] {
        geocad::geometry::Vector2D a(1.0, 0.0);
        geocad::geometry::Vector2D b(0.0, 1.0);
        EXPECT_NEAR(90.0, geocad::engine::GeometryEngine::angleDegrees(a, b), 1e-12);
    });
}
