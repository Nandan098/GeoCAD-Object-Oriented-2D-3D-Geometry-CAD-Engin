#include "TestFramework.h"

#include "geocad/engine/CollisionDetector.h"
#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Rectangle.h"

void runCollisionTests()
{
    geocad_test::run("circle-circle collision", [] {
        geocad::shapes::Circle first("a", 0, 0, 5);
        geocad::shapes::Circle second("b", 7, 0, 5);
        EXPECT_TRUE(geocad::engine::CollisionDetector::collides(first, second));
    });

    geocad_test::run("circle-circle separation", [] {
        geocad::shapes::Circle first("a", 0, 0, 2);
        geocad::shapes::Circle second("b", 5, 0, 2);
        EXPECT_FALSE(geocad::engine::CollisionDetector::collides(first, second));
    });

    geocad_test::run("circle-rotated rectangle collision", [] {
        geocad::shapes::Circle circle("c", 0, 1.5, 1.0);
        geocad::shapes::Rectangle rectangle("r", 0, 0, 4, 2, 45);
        EXPECT_TRUE(geocad::engine::CollisionDetector::collides(circle, rectangle));
    });

    geocad_test::run("rotated rectangle SAT", [] {
        geocad::shapes::Rectangle first("a", 0, 0, 4, 2, 45);
        geocad::shapes::Rectangle second("b", 0, 0, 4, 2, -45);
        EXPECT_TRUE(geocad::engine::CollisionDetector::collides(first, second));
    });
}
