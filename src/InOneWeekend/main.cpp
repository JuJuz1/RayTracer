#include <iostream>
#include <vec3.h>
#include <color.h>

int main() {

    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - 1 - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            const double b = 0.0;
            const auto pixel_color = color{
                static_cast<double>(i) / (image_width - 1), 
                static_cast<double>(j) / (image_height - 1), b};
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}