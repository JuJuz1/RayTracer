#include <iostream>
#include <limits>

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"

// Calculates the color of a pixel with a given ray from the camera
color ray_color(const Ray& r, const Hittable_list& world) {
    Hit_record rec;
    if (world.hit(r, 0, std::numeric_limits<double>::infinity(), rec)) {
        return (rec.normal + color{1, 1, 1}) * 0.5;
    }

    // Normalize the ray and get the unit vector
    const Vec3 unit_direction{unit_vector(r.direction())};
    // Linear interpolation by scaling the y-coordinate to the range [0, 1]
    const double a = 0.5 * (unit_direction.y() + 1.0);
    // From blue to white for now...
    return color{1.0, 1.0, 1.0} * (1.0 - a) + color{0.5, 0.7, 1.0} * a;
}

int main() {
    // Image

    constexpr float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    // Calculate height, has to be at least 1 pixel
    constexpr int image_height = std::max(static_cast<int>(image_width / aspect_ratio), 1);

    // World

    Hittable_list world;
    // This is very strange, for example radius of 0.88 makes the sphere almost 2 times smaller
    // Values below 0.8 don't even produce the sphere at all...
    world.add(std::make_unique<Sphere>(Point3{0, 0, -1.0}, 0.9));
    world.add(std::make_unique<Sphere>(Point3{0, -100.5, -1}, 100));

    // Camera
    
    // Distance from the eye to the viewport
    const double focal_length = 1.0;
    
    constexpr double viewport_height = 2.0;
    // Determine viewport_width from the actual image size, can't be perfect in terms of aspect_ratio
    constexpr double viewport_width = viewport_height * (image_width / image_height);
    
    // Calculate the vectors for horizontal and vertical traversing of the viewport
    const Vec3 viewport_u{viewport_width, 0, 0};
    const Vec3 viewport_v{0, -viewport_height, 0};
    
    // Horizontal and vertical delta vectors from pixel to pixel
    const Vec3 pixel_delta_u{viewport_u / image_width};
    const Vec3 pixel_delta_v{viewport_v / image_height};
    
    const Point3 camera_center{0, 0, 0};
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
            
            const color pixel_color{ray_color(r, world)};
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}