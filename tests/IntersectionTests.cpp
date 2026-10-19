#include "TestFramework.h"

#include "geocad/engine/IntersectionEngine.h"
#include "geocad/geometry/Line2D.h"
#include "geocad/shapes/Circle.h"
#include "geocad/shapes/Polygon.h"

void runIntersectionTests()
{
    geocad_test::run("line segment intersection", [] {
        geocad::geometry::Line2D first({0.0, 0.0}, {10.0, 10.0});
        geocad::geometry::Line2D second({0.0, 10.0}, {10.0, 0.0});
        const auto result = geocad::engine::IntersectionEngine::intersectLines(first, second);
        EXPECT_EQ(geocad::engine::LineIntersectionType::Point, result.type);
        EXPECT_TRUE(result.point.has_value());
        EXPECT_NEAR(5.0, result.point->x(), 1e-12);
        EXPECT_NEAR(5.0, result.point->y(), 1e-12);
    });

    geocad_test::run("parallel lines", [] {
        geocad::geometry::Line2D first({0.0, 0.0}, {10.0, 0.0});
        geocad::geometry::Line2D second({0.0, 2.0}, {10.0, 2.0});
        const auto result = geocad::engine::IntersectionEngine::intersectLines(first, second);
        EXPECT_EQ(geocad::engine::LineIntersectionType::Parallel, result.type);
    });

    geocad_test::run("circle intersections", [] {
        geocad::shapes::Circle first("c1", 0.0, 0.0, 5.0);
        geocad::shapes::Circle second("c2", 6.0, 0.0, 5.0);
        const auto points = geocad::engine::IntersectionEngine::intersectCircles(first, second);
        EXPECT_EQ(static_cast<std::size_t>(2), points.size());
        EXPECT_NEAR(3.0, points[0].x(), 1e-12);
        EXPECT_NEAR(4.0, std::abs(points[0].y()), 1e-12);
    });

    geocad_test::run("point in polygon", [] {
        geocad::shapes::Polygon polygon("p1", {{0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0}, {0.0, 10.0}});
        EXPECT_TRUE(geocad::engine::IntersectionEngine::pointInPolygon({5.0, 5.0}, polygon));
        EXPECT_FALSE(geocad::engine::IntersectionEngine::pointInPolygon({15.0, 5.0}, polygon));
    });
}
