#pragma once

#include "geocad/geometry/Point3D.h"
#include "geocad/shapes/Shape.h"

namespace geocad::shapes {

class Sphere : public Shape {
public:
    Sphere(const std::string& id, double x, double y, double z, double radius);

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

    const geocad::geometry::Point3D& center() const;
    double radius() const;

private:
    geocad::geometry::Point3D center_;
    double radius_{1.0};
};

} // namespace geocad::shapes
