#ifndef INCLUDE_COLOR_H_
#define INCLUDE_COLOR_H_

#include <fstream>
#include <vector>

#include "vec3.h"

using Color = Vec3;

// Various predefined colors made with:
// https://rgbcolorpicker.com/0-1
namespace colors {

constexpr Vec3 Red       {1.0, 0.0, 0.0};
constexpr Vec3 Orange    {0.8, 0.6, 0.2};
constexpr Vec3 Brown     {0.7, 0.6, 0.5};
constexpr Vec3 Yellow    {1.0, 0.9, 0.15};
constexpr Vec3 Pink      {0.8, 0.1, 0.7};
constexpr Vec3 Green     {0.0, 1.0, 0.0};
constexpr Vec3 Lime      {0.7, 0.8, 0.1};
constexpr Vec3 Blue      {0.0, 0.0, 1.0};
constexpr Vec3 LightBlue {0.5, 0.7, 1.0};
constexpr Vec3 DarkBlue  {0.1, 0.2, 0.5};
constexpr Vec3 White     {1.0, 1.0, 1.0};
constexpr Vec3 Gray      {0.5, 0.5, 0.5};
constexpr Vec3 LightGray {0.8, 0.8, 0.8};
constexpr Vec3 Black     {0.0, 0.0, 0.0};

} // namespace colors

// Returns the gamma 2 transform for a linear component
double linear_to_gamma_two(double linear_component) noexcept;

// Returns x transformed to gamma 2 and byte range [0, 255]
int to_gamma_and_byte_range(double x) noexcept;

// Single-threaded
// Calls write_color_to_stream for color
void write_color(std::ofstream& out, const Color& color);

// Multithreading
// Calls write_color_to_stream for every color x y z in buffer
void write_color_from_buffer(std::ofstream& out, const std::vector<Color>& color_buffer);

// Writes the color to the stream
void write_color_to_stream(std::ofstream& out, double x, double y, double z);

#endif // INCLUDE_COLOR_H_
