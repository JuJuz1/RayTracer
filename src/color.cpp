#include "color.h"

#include <cmath>
#include <cstdio>

#include "interval.h"

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

void write_color(std::ofstream& out, const Color& pixel_color) {
    const double r = linear_to_gamma_two(pixel_color.x());
    const double g = linear_to_gamma_two(pixel_color.y());
    const double b = linear_to_gamma_two(pixel_color.z());
    
    // constexpr double multiplier = 256;
    // static const Interval intensity{ 0.000, 0.999 };
    // const int rbyte = static_cast<int>(intensity.clamp(r) * multiplier);
    // const int gbyte = static_cast<int>(intensity.clamp(g) * multiplier);
    // const int bbyte = static_cast<int>(intensity.clamp(b) * multiplier);
    // out << rbyte << ' ' << gbyte << ' ' << bbyte << "\n";
    
    // Translate from range [0, 1] to byte range [0, 255]
    // Use this for optimization reasons
    const int rbyte = static_cast<int>(std::fmin(std::fmax(r, 0.000), 0.999) * 256);
    const int gbyte = static_cast<int>(std::fmin(std::fmax(g, 0.000), 0.999) * 256);
    const int bbyte = static_cast<int>(std::fmin(std::fmax(b, 0.000), 0.999) * 256);
    
    // A faster way of writing to the file
    // 13 should be enough: XXX XXX XXX\n -> 13 including null terminator
    char buf[13];
    const int len{ std::snprintf(buf, sizeof(buf), "%d %d %d\n", rbyte, gbyte, bbyte) };
    out.write(buf, len);
}