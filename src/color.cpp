#include <color.h>

void write_color(std::ostream& out, const color& pixel_color) {
    // Translate from range [0, 1] to byte range [0, 255]
    const int rbyte = static_cast<int>(255.999 * pixel_color.x());
    const int gbyte = static_cast<int>(255.999 * pixel_color.y());
    const int bbyte = static_cast<int>(255.999 * pixel_color.z());

    out << rbyte << ' ' << gbyte << ' ' << bbyte << "\n";
}