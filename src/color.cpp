#include <color.h>

void write_color(std::ostream& out, const color& pixel_color) {
    const double multiplier = 255.999;
    // Translate from range [0, 1] to byte range [0, 255]
    const int rbyte = static_cast<int>(pixel_color.x() * multiplier);
    const int gbyte = static_cast<int>(pixel_color.y() * multiplier);
    const int bbyte = static_cast<int>(pixel_color.z() * multiplier);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << "\n";
}