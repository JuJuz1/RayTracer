#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <random>

namespace rt {

    // Constants

    constexpr double infinity = std::numeric_limits<double>::infinity();
    constexpr double pi = 3.1415926535897932385;

    // Utility Functions

    inline constexpr double degrees_to_radians(double degrees) noexcept {
        return degrees * pi / 180.0;
    }

    // Returns a random double in the range [0.0, 1.0[
    inline double random_double() noexcept {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    // Returns a random double in the range [min, max[
    inline double random_double(double min, double max) noexcept {
        return min + (max - min) * random_double();
    }

} // namespace rt

#endif