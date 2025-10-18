#ifndef INCLUDE_RTWEEKEND_H_
#define INCLUDE_RTWEEKEND_H_

#include <limits>
#include <numbers>
#include <random>
#include <iostream>
#include <iomanip>

#include "vec3.h"

namespace rt {

// Constants

constexpr double infinity{ std::numeric_limits<double>::infinity() };
constexpr double pi{ std::numbers::pi };

// Utility Functions

[[nodiscard]] constexpr double degrees_to_radians(double degrees) noexcept {
    return degrees * pi / 180.0;
}

// Returns a random double in the range [0.0, 1.0[
[[nodiscard]] inline double random_double() noexcept {
    static std::random_device dev;
    static std::mt19937 generator{ dev() };
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// Returns a random double in the range [min, max[
[[nodiscard]] inline double random_double(double min, double max) noexcept {
    return min + (max - min) * random_double();
}

} // namespace rt

#endif // INCLUDE_RTWEEKEND_H_
