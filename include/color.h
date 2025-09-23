#ifndef COLOR_H
#define COLOR_H

#include <fstream>

#include "vec3.h"

using Color = Vec3;

// Various predefined colors
struct Colors {
    static const Vec3 Red;       // {1.0, 0.0, 0.0};
    static const Vec3 Orange;    // {0.8, 0.6, 0.2};
    static const Vec3 Brown;     // {0.7, 0.6, 0.5};
    static const Vec3 Yellow;    // {1.0, 0.9, 0.15};
    static const Vec3 Pink;      // {0.8, 0.1, 0.7};
    static const Vec3 Green;     // {0.0, 1.0, 0.0};
    static const Vec3 Lime;      // {0.7, 0.8, 0.1};
    static const Vec3 Blue;      // {0.0, 0.0, 1.0};
    static const Vec3 LightBlue; // {0.5, 0.7, 1.0};
    static const Vec3 DarkBlue;  // {0.1, 0.2, 0.5};
    static const Vec3 White;     // {1.0, 1.0, 1.0};
    static const Vec3 Gray;      // {0.5, 0.5, 0.5};
    static const Vec3 LightGray; // {0.8, 0.8, 0.8};
    static const Vec3 Black;     // {0.0, 0.0, 0.0};
};


// Returns the gamma 2 transform for a linear component
double linear_to_gamma_two(double linear_component) noexcept;

// Returns x transformed to gamma 2 and byte range [0, 255]
int to_gamma_and_byte_range(double x) noexcept;

void write_color(std::ofstream& out, const Color& color);

// Multithreading "overload"
// Calls write_color_to_stream(std::fstream& out, double x, double y, double z)
// for every x y z of every Color in buffer
void write_color(std::ofstream& out, const Color* const buffer, uint32_t len);

// Writes the color to the stream
void write_color_to_stream(std::ofstream& out, double x, double y, double z);

#endif