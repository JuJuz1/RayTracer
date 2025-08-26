#include <iostream>
#include <color.h>
#include <ray.h>

color ray_color(const Ray& r) {
    return color{0, 0, 0};
}

int main() {

    // Image

    constexpr float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    // Calculate height, has to be at least 1 pixel
    constexpr int image_height = std::max(static_cast<int>(image_width / aspect_ratio), 1);

    // Camera

    // Distance from the eye to the viewport
    const double focal_length = 1.0;
    
    constexpr double viewport_height = 2.0;
    // Determine viewport_width from the actual image size, can't be perfect in terms of aspect_ratio
    // but gets pretty close nonetheless
    constexpr double viewport_width = viewport_height * (image_width / image_height);
    const Point3 camera_center{0, 0, 0};

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    const Vec3 viewport_u{viewport_width, 0, 0}; // Left to right
    const Vec3 viewport_v{0, -viewport_height, 0}; // Up to down

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    const Vec3 pixel_delta_u = viewport_u / image_width;
    const Vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    const Point3 viewport_upper_left{
        camera_center - Vec3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2};
    const Point3 pixel00_loc{viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5};

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - 1 - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            const Point3 pixel_center{pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j)};
            const Vec3 ray_direction{pixel_center - camera_center};
            const Ray r{camera_center, ray_direction};

            const color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}