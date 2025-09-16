#include <cmath>

#include "color.h"
#include "interval.h"

// Colors
const Vec3 Color::Red       {1.0, 0.0, 0.0};
const Vec3 Color::Green     {0.0, 1.0, 0.0};
const Vec3 Color::Blue      {0.0, 0.0, 1.0};
const Vec3 Color::LightBlue {0.5, 0.7, 1.0};
const Vec3 Color::White     {1.0, 1.0, 1.0};
const Vec3 Color::Gray      {0.5, 0.5, 0.5};
const Vec3 Color::Black     {0.0, 0.0, 0.0};

constexpr double linear_to_gamma_two(double linear_component) noexcept {
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    constexpr double multiplier = 256;

    const double r = linear_to_gamma_two(pixel_color.x());
    const double g = linear_to_gamma_two(pixel_color.y());
    const double b = linear_to_gamma_two(pixel_color.z());

    // Translate from range [0, 1] to byte range [0, 255]
    static const Interval intensity(0.000, 0.999);
    const int rbyte = static_cast<int>(intensity.clamp(r) * multiplier);
    const int gbyte = static_cast<int>(intensity.clamp(g) * multiplier);
    const int bbyte = static_cast<int>(intensity.clamp(b) * multiplier);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << "\n";
}