#include "color.h"

#include <cmath>
#include <cstdio>
#include <vector>

double linear_to_gamma_two(double linear_component) noexcept {
    if (0 < linear_component) [[likely]]
        return std::sqrt(linear_component);

    return 0;
}

int to_gamma_and_byte_range(double x) noexcept {
    return static_cast<int>(std::fmin(std::fmax(linear_to_gamma_two(x), 0.000), 0.999) * 256);
}

void write_color(std::ofstream& out, const Color& pixel_color) {
    write_color_to_stream(out, pixel_color.x(), pixel_color.y(), pixel_color.z());
}

void write_color_from_buffer(std::ofstream& out, const std::vector<Color>& color_buffer) {
    for (const auto& color : color_buffer) {
        write_color_to_stream(out, color.x(), color.y(), color.z());
    }
}

void write_color_to_stream(std::ofstream& out, double x, double y, double z) {
    // Translate from range [0, 1] to byte range [0, 255]
    const int rbyte{ to_gamma_and_byte_range(x) };
    const int gbyte{ to_gamma_and_byte_range(y) };
    const int bbyte{ to_gamma_and_byte_range(z) };

    // A faster way of writing to the file
    // 13 should be enough: XXX XXX XXX\n -> 13 including null terminator
    // Gets converted into characters so stays in range [0-127]
    char buf[13];
    const int len{ std::snprintf(buf, sizeof(buf), "%d %d %d\n", rbyte, gbyte, bbyte) };
    out.write(buf, len);
}
