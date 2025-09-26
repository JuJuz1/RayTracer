#include "color.h"

#include <cmath>
#include <cstdio>
#include <vector>

// https://rgbcolorpicker.com/0-1

const Vec3 Colors::Red       {1.0, 0.0, 0.0};
const Vec3 Colors::Orange    {0.8, 0.6, 0.2};
const Vec3 Colors::Brown     {0.7, 0.6, 0.5};
const Vec3 Colors::Yellow    {1.0, 0.9, 0.15};
const Vec3 Colors::Pink      {0.8, 0.1, 0.7};
const Vec3 Colors::Green     {0.0, 1.0, 0.0};
const Vec3 Colors::Lime      {0.7, 0.8, 0.1};
const Vec3 Colors::Blue      {0.0, 0.0, 1.0};
const Vec3 Colors::LightBlue {0.5, 0.7, 1.0};
const Vec3 Colors::DarkBlue  {0.1, 0.2, 0.5};
const Vec3 Colors::White     {1.0, 1.0, 1.0};
const Vec3 Colors::Gray      {0.5, 0.5, 0.5};
const Vec3 Colors::LightGray {0.8, 0.8, 0.8};
const Vec3 Colors::Black     {0.0, 0.0, 0.0};

double linear_to_gamma_two(double linear_component) noexcept {
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

int to_gamma_and_byte_range(double x) noexcept {
    return static_cast<int>(std::fmin(std::fmax(linear_to_gamma_two(x), 0.000), 0.999) * 256);
}

void write_color(std::ofstream& out, const Color& pixel_color) {
    write_color_to_stream(out, pixel_color.x(), pixel_color.y(), pixel_color.z());
}

void write_color(std::ofstream& out, const std::vector<Color>& color_buffer) {
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
