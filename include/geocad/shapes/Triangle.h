#pragma once

#include "geocad/geometry/Point2D.h"
#include "geocad/shapes/Shape.h"

#include <array>

namespace geocad::shapes {

class Triangle : public Shape {
public:
    Triangle(const std::string& id, const geocad::geometry::Point2D& a,
             const geocad::geometry::Point2D& b, const geocad::geometry::Point2D& c);

    std::string typeName() const override;
    ShapeDimension dimension() const override;
    double area() const override;
    double perimeter() const override;
    double volume() const override;
    geocad::geometry::Point3D referencePoint() const override;
    geocad::geometry::BoundingBox3D boundingBox() const override;

    void translate(const geocad::geometry::Vector3D& delta) override;
    void rotate(double rxDegrees, double ryDegrees, double rzDegrees) override;
    void scale(double factor) override;

    std::unique_ptr<Shape> clone() const override;
    std::string serialize() const override;

    const std::array<geocad::geometry::Point2D, 3>& vertices() const;

private:
    std::array<geocad::geometry::Point2D, 3> vertices_;
};

} // namespace geocad::shapes
