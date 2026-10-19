#pragma once

#include "geocad/geometry/Point2D.h"
#include "geocad/shapes/Shape.h"

#include <vector>

namespace geocad::shapes {

class Polygon : public Shape {
public:
    Polygon(const std::string& id, const std::vector<geocad::geometry::Point2D>& vertices);

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

    const std::vector<geocad::geometry::Point2D>& vertices() const;

private:
    std::vector<geocad::geometry::Point2D> vertices_;
};

} // namespace geocad::shapes
