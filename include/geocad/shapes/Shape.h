#pragma once

#include "geocad/geometry/BoundingBox3D.h"
#include "geocad/geometry/Point3D.h"
#include "geocad/geometry/Vector3D.h"

#include <memory>
#include <string>

namespace geocad::shapes {

enum class ShapeDimension {
    TwoD,
    ThreeD
};

class Shape {
public:
    explicit Shape(std::string id);
    virtual ~Shape() = default;

    const std::string& id() const;

    virtual std::string typeName() const = 0;
    virtual ShapeDimension dimension() const = 0;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual double volume() const = 0;
    virtual geocad::geometry::Point3D referencePoint() const = 0;
    virtual geocad::geometry::BoundingBox3D boundingBox() const = 0;

    virtual void translate(const geocad::geometry::Vector3D& delta) = 0;
    virtual void rotate(double rxDegrees, double ryDegrees, double rzDegrees) = 0;
    virtual void scale(double factor) = 0;

    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual std::string serialize() const = 0;

protected:
    std::string id_;
};

using ShapePtr = std::unique_ptr<Shape>;

} // namespace geocad::shapes
