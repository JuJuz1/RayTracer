#include <string>
#include <iomanip>
#include <chrono>

#include "camera.h"
#include "rtweekend.h"
#include "material.h"

void Camera::render(const Hittable& world) noexcept {
    initialize();

    auto print_property_formatted = [](const std::string& property, int value){
        constexpr int format_width_left{ 18 };
        constexpr int format_width_right{ 3 };
        std::clog << std::left << std::setw(format_width_left) << property 
                  << std::right << std::setw(format_width_right) << value << "\n";
    };

    std::clog << "\nRendering an image with properties:\n";
    print_property_formatted("Width", image_width);
    print_property_formatted("Height", image_height);
    print_property_formatted("Samples per pixel", samples_per_pixel);
    print_property_formatted("Max depth", max_depth);
    std::clog << "\n";

    using namespace std::chrono;
    const auto start = high_resolution_clock::now();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j{ 0 }; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - 1 - j) << ' ' << std::flush;
        for (int i{ 0 }; i < image_width; ++i) {
            color pixel_color{0, 0, 0};
            for (int sample{ 0 }; sample < samples_per_pixel; ++sample) {
                const Ray r{ get_ray(i, j) };
                pixel_color += ray_color(r, max_depth, world);
            }
            
            write_color(std::cout, pixel_color * pixel_sample_scale);
        }
    }

    const auto end{ high_resolution_clock::now() }; 
    const auto ms{ duration_cast<milliseconds>(end - start).count() };
    std::clog << "\nRendering took: " << std::setprecision(3) << ms / 1000.0 << "s";
}

void Camera::initialize() noexcept {
    image_height = std::max(static_cast<int>(image_width / aspect_ratio), 1);

    pixel_sample_scale = 1.0 / samples_per_pixel;
    
    constexpr double viewport_height = 2.0;
    
    // Determine viewport_width from the actual image size, can't be perfect in terms of aspect_ratio
    const double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the vectors for horizontal and vertical traversing of the viewport
    const Vec3 viewport_u{ viewport_width, 0, 0 };
    const Vec3 viewport_v{ 0, -viewport_height, 0 };
    
    // Horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;
    
    // Distance from the eye to the viewport
    constexpr double focal_length = 1.0;

    center = Point3{ 0, 0, 0 };
    const Point3 viewport_upper_left{
        center - Vec3{ 0, 0, focal_length } - viewport_u / 2 - viewport_v / 2 };
    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
}

color Camera::ray_color(const Ray& r, int depth, const Hittable& world) const noexcept {
    // Hit ray bounce limit (max_depth)
    if (depth <= 0)
        return Color::Black;
    
    Hit_record rec;
    // If they ray's origin is just below the surface it might hit the surface immediately
    // 0.001 ignores hits that are very close
    if (world.hit(r, Interval{ 0.001, rt::infinity }, rec)) {
        Ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, depth - 1, world);
        
        return Color::Black;
    }

    const Vec3 unit_direction{ unit_vector(r.direction()) };
    // Linear interpolation by scaling the y-coordinate to the range [0, 1]
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return Color::White * (1.0 - a) + Color::LightBlue * a;
}

Ray Camera::get_ray(int i, int j) const noexcept {
    const Vec3 offset{ sample_square() };
    const Point3 pixel_sample{pixel00_loc
                           + (pixel_delta_u * (i + offset.x()))
                           + (pixel_delta_v * (j + offset.y()))};

    return Ray{ center, pixel_sample - center };
}

Vec3 Camera::sample_square() const noexcept {
    return Vec3{ rt::random_double() - 0.5, rt::random_double() - 0.5, 0 };
}