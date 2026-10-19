#pragma once

namespace geocad::geometry {

class Point3D {
public:
    Point3D() = default;
    Point3D(double x, double y, double z);

    double x() const;
    double y() const;
    double z() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

private:
    double x_{0.0};
    double y_{0.0};
    double z_{0.0};
};

} // namespace geocad::geometry
