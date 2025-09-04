#include "camera.h"
#include "rtweekend.h"

void Camera::render(const Hittable& world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - 1 - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color{0, 0, 0};
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                const Ray r{get_ray(i, j)};
                pixel_color += ray_color(r, world);
            }

            write_color(std::cout, pixel_color * pixel_sample_scale);
        }
        
        // for (int i = 0; i < image_width; ++i) {
        //     const Point3 pixel_center{pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j)};
        //     const Vec3 ray_direction{pixel_center - center};
        //     const Ray r{center, ray_direction};

        //     const color pixel_color{ray_color(r, world)};
        //     write_color(std::cout, pixel_color);
        // }
    }

    std::clog << "\nDone.\n";
}

void Camera::initialize() {
    image_height = std::max(static_cast<int>(image_width / aspect_ratio), 1);

    pixel_sample_scale = 1.0 / samples_per_pixel;

    // Distance from the eye to the viewport
    constexpr double focal_length = 1.0;
    
    constexpr double viewport_height = 2.0;
    // Determine viewport_width from the actual image size, can't be perfect in terms of aspect_ratio
    const double viewport_width = viewport_height * (image_width / image_height);

    // Calculate the vectors for horizontal and vertical traversing of the viewport
    const Vec3 viewport_u{viewport_width, 0, 0};
    const Vec3 viewport_v{0, -viewport_height, 0};
    
    // Horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;
    
    center = Point3{0, 0, 0};
    const Point3 viewport_upper_left{
        center - Vec3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2};
    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
}

color Camera::ray_color(const Ray& r, const Hittable& world) const {
    Hit_record rec;
    if (world.hit(r, Interval{0.0, rt::infinity}, rec))
        return (rec.normal + color{1, 1, 1}) * 0.5;

    const Vec3 unit_direction{unit_vector(r.direction())};
    // Linear interpolation by scaling the y-coordinate to the range [0, 1]
    const double a = 0.5 * (unit_direction.y() + 1.0);
    // From blue to white for now...
    // TODO: make a color enum or smth to have the colors
    // Access with t. ex. COLOR.Blue?
    return color{1.0, 1.0, 1.0} * (1.0 - a) + color{0.5, 0.7, 1.0} * a;
}

Ray Camera::get_ray(int i, int j) const {
    const Vec3 offset{sample_square()};
    const Point3 pixel_sample{pixel00_loc
                        + (pixel_delta_u * (i + offset.x()))
                        + (pixel_delta_v * (j + offset.y()))};

    return Ray{center, pixel_sample - center};
}

Vec3 Camera::sample_square() const {
    return Vec3(rt::random_double() - 0.5, rt::random_double() - 0.5, 0);
}