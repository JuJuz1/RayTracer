#include "color.h"
#include "interval.h"

void write_color(std::ostream& out, const color& pixel_color) {
    constexpr double multiplier = 256;

    // Translate from range [0, 1] to byte range [0, 255]
    static const Interval intensity(0.000, 0.999);
    const int rbyte = static_cast<int>(intensity.clamp(pixel_color.x()) * multiplier);
    const int gbyte = static_cast<int>(intensity.clamp(pixel_color.y()) * multiplier);
    const int bbyte = static_cast<int>(intensity.clamp(pixel_color.z()) * multiplier);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << "\n";
}