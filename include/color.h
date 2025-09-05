#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

using color = Vec3;

// Various predefined colors
struct Color {
    static const Vec3 Red;
    static const Vec3 Green;
    static const Vec3 Blue;
    static const Vec3 LightBlue;
    static const Vec3 White;
    static const Vec3 Gray;
    static const Vec3 Black;
};

void write_color(std::ostream& out, const color& pixel_color);

#endif