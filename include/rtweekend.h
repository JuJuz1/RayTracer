#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <random>
#include <string>
#include <iomanip>

#include "vec3.h"

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

    // Camera

    // A utility function to print camera properties
    template <typename T>
    inline void print_camera_property_formatted(const std::string& property, const T& value) {
        constexpr int format_width_left{ 18 }; // Length of "Samples per pixel" + 1
        constexpr int format_width_right{ 3 };
        std::clog << std::left << std::setw(format_width_left) << property 
                  << std::right << std::setw(format_width_right) << value << "\n";
    };

    // Overload for Vec3
    inline void print_camera_property_formatted(const std::string& property, const Vec3& v) {
        constexpr int format_width_left{ 15 };
        constexpr int format_width_component{ 2 };
        std::clog << std::left << std::setw(format_width_left) << property
                  << std::right << std::setw(format_width_component) << v.x() << ", "
                  << std::right << std::setw(format_width_component) << v.y() << ", "
                  << std::right << std::setw(format_width_component) << v.z() << "\n";
    }

} // namespace rt

#endif