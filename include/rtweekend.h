#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>

namespace rt {

    // Contants

    static constexpr double infinity = std::numeric_limits<double>::infinity();
    static constexpr double pi = 3.1415926535897932385;

    // Utility Functions

    inline constexpr double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }

} // namespace rt

#endif