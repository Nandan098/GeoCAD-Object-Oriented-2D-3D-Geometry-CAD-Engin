#pragma once

namespace geocad::geometry {

class Point2D {
public:
    Point2D() = default;
    Point2D(double x, double y);

    double x() const;
    double y() const;

    void setX(double x);
    void setY(double y);

private:
    double x_{0.0};
    double y_{0.0};
};

} // namespace geocad::geometry
