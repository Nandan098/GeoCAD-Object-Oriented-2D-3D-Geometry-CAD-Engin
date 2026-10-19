#pragma once

#include <cmath>

namespace geocad::geometry {

constexpr double kPi = 3.1415926535897932384626433832795;
constexpr double kEpsilon = 1e-9;

inline double degreesToRadians(double degrees)
{
    return degrees * kPi / 180.0;
}

inline double radiansToDegrees(double radians)
{
    return radians * 180.0 / kPi;
}

inline bool approximatelyEqual(double a, double b, double epsilon = kEpsilon)
{
    return std::abs(a - b) <= epsilon;
}

} // namespace geocad::geometry
