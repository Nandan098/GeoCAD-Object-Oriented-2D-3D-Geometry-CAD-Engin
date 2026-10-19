#include "TestFramework.h"

#include "geocad/geometry/Constants.h"
#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Cube.h"
#include "geocad/shapes/Rectangle.h"
#include "geocad/shapes/Sphere.h"

void runShapeTests()
{
    geocad_test::run("circle area/perimeter", [] {
        geocad::shapes::Circle circle("c1", 0.0, 0.0, 2.0);
        EXPECT_NEAR(4.0 * geocad::geometry::kPi, circle.area(), 1e-12);
        EXPECT_NEAR(4.0 * geocad::geometry::kPi, circle.perimeter(), 1e-12);
    });

    geocad_test::run("rotated rectangle bounding box", [] {
        geocad::shapes::Rectangle rectangle("r1", 0.0, 0.0, 2.0, 4.0, 90.0);
        const auto box = rectangle.boundingBox();
        EXPECT_NEAR(-2.0, box.minimum().x(), 1e-12);
        EXPECT_NEAR(2.0, box.maximum().x(), 1e-12);
        EXPECT_NEAR(-1.0, box.minimum().y(), 1e-12);
        EXPECT_NEAR(1.0, box.maximum().y(), 1e-12);
    });

    geocad_test::run("sphere volume", [] {
        geocad::shapes::Sphere sphere("s1", 0.0, 0.0, 0.0, 3.0);
        EXPECT_NEAR(36.0 * geocad::geometry::kPi, sphere.volume(), 1e-12);
    });

    geocad_test::run("cube volume and surface area", [] {
        geocad::shapes::Cube cube("cube1", 0.0, 0.0, 0.0, 2.0);
        EXPECT_NEAR(8.0, cube.volume(), 1e-12);
        EXPECT_NEAR(24.0, cube.area(), 1e-12);
    });

    geocad_test::run("circle transformations", [] {
        geocad::shapes::Circle circle("c1", 1.0, 2.0, 2.0);
        circle.translate(geocad::geometry::Vector3D(3.0, 4.0, 0.0));
        circle.scale(2.0);
        const auto point = circle.referencePoint();
        EXPECT_NEAR(4.0, point.x(), 1e-12);
        EXPECT_NEAR(6.0, point.y(), 1e-12);
        EXPECT_NEAR(4.0, circle.radius(), 1e-12);
    });
}
