#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

using color = Vec3;

// Various predefined colors
struct Color {
    static const Vec3 Red;       // {1.0, 0.0, 0.0};
    static const Vec3 Green;     // {0.0, 1.0, 0.0};
    static const Vec3 Blue;      // {0.0, 0.0, 1.0};
    static const Vec3 LightBlue; // {0.5, 0.7, 1.0};
    static const Vec3 White;     // {1.0, 1.0, 1.0};
    static const Vec3 Gray;      // {0.5, 0.5, 0.5};
    static const Vec3 Black;     // {0.0, 0.0, 0.0};
};

void write_color(std::ostream& out, const color& pixel_color);

#endif