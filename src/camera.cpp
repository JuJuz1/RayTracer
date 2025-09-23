#include "camera.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <cmath>

#include "color.h"
#include "rtweekend.h"
#include "material.h"

bool Camera::render(const Hittable& world, const std::string& filename) noexcept {
    initialize();
    print_properties();

    std::ofstream out{ "image.ppm" };
    // Open for output and clear existing content
    if (!out.is_open()) {
        std::cerr << "Error while opening file!\n";
        return false;
    }

    using namespace std::chrono;
    const auto start{ high_resolution_clock::now() };
    auto last_print{ start };
    constexpr double progress_refresh_rate{ 0.5 };

    // Render
    std::cout << "Rendering output to file: " << filename << "\n";
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // TODO: thread this, painfully slow to render final image otherwise
    for (int j{ 0 }; j < image_height; ++j) {
        for (int i{ 0 }; i < image_width; ++i) {
            Color pixel_color{};
            for (int sample{ 0 }; sample < samples_per_pixel; ++sample) {
                const Ray r{ get_ray(i, j) };
                pixel_color += trace_ray(r, max_depth, world);
            }
            
            write_color(out, pixel_color * pixel_sample_scale);
        }
        
        // Progress indicators
        const auto now{ high_resolution_clock::now() };
        double seconds_since_last{ duration<double>(now - last_print).count() };
        if (progress_refresh_rate < seconds_since_last) {
            const int scanlines_done{ j + 1 };
            const int scanlines_remaining{ image_height - scanlines_done };

            const double elapsed{ duration<double>(now - start).count() };
            const double eta{ elapsed / scanlines_done * scanlines_remaining };
            const double eta_min{ eta / 60.0 };

            std::cout << "\rScanlines remaining: " << (image_height - 1 - j)
                      << " | Elapsed time: " << std::fixed << std::setprecision(3) << elapsed << "s" 
                      << " | ETA: " << eta << "s" << " (" << std::setprecision(1) << eta_min << "m)"
                      << ' ' // Padding
                      << std::flush;
            
            last_print = now;
        }
    }
    
    out.close();
    return true;
}

void Camera::initialize() noexcept {
    image_height = std::max(static_cast<int>(image_width / aspect_ratio), 1);

    pixel_sample_scale = 1.0 / samples_per_pixel;

    center = lookfrom;

    // Viewport dimensions
    const double theta{ rt::degrees_to_radians(vfov) };
    const double h{ std::tan(theta / 2) };
    const double viewport_height{ 2 * h * focus_dist };
    // Determine viewport_width from the actual image size, can't be perfect in terms of aspect_ratio
    const double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    // Camera unit basis vectors
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors for horizontal and vertical traversing of the viewport
    const Vec3 viewport_u{ u * viewport_width };
    const Vec3 viewport_v{ -v * viewport_height };
    
    // Horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;
    
    const Point3 viewport_upper_left{
        center - (w * focus_dist) - viewport_u / 2 - viewport_v / 2 };
    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

    // Defocus disk basis vectors
    const double defocus_radius{ focus_dist * std::tan(rt::degrees_to_radians(defocus_angle / 2)) };
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

void Camera::print_properties() const noexcept {
    std::cout << "\nRendering an image with properties:\n";
    rt::print_camera_property_formatted("Width", image_width);
    rt::print_camera_property_formatted("Height", image_height);
    rt::print_camera_property_formatted("Samples per pixel", samples_per_pixel);
    rt::print_camera_property_formatted("Max depth", max_depth);

    std::cout << "\nViewport properties:\n";
    rt::print_camera_property_formatted("Vertical fov", vfov);
    rt::print_camera_property_formatted("Look from", lookfrom);
    rt::print_camera_property_formatted("Look at", lookat);
    
    std::cout << "\nLens properties:\n";
    rt::print_camera_property_formatted("Defocus angle", defocus_angle);
    rt::print_camera_property_formatted("Focus distance", focus_dist);
    std::cout << "\n";
}

Color Camera::trace_ray(const Ray& r, int depth, const Hittable& world) const noexcept {
    // Hit ray bounce limit (max_depth)
    if (depth <= 0)
        return Colors::Black;
    
    HitRecord rec;
    // If they ray's origin is just below the surface it might hit the surface immediately
    // An interval with min of 0.001 ignores hits that are very close
    if (world.hit(r, Interval{ 0.001, rt::infinity }, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * trace_ray(scattered, depth - 1, world);
        
        return Colors::Black;
    }

    // Nothing was hit -> render background
    const Vec3 unit_direction{ unit_vector(r.direction()) };
    // Linear interpolation by scaling the y-coordinate to the range [0, 1]
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return background_color_top * a + background_color_bottom * (1.0 - a);
}

Ray Camera::get_ray(int i, int j) const noexcept {
    const Vec3 offset{ sample_square() };
    const Point3 pixel_sample{pixel00_loc
                           + (pixel_delta_u * (i + offset.x()))
                           + (pixel_delta_v * (j + offset.y()))};
    
    const Point3 ray_origin{ (defocus_angle <= 0) ? center : defocus_disk_sample() };
    const Vec3 ray_direction{ pixel_sample - ray_origin };
    return Ray{ ray_origin, ray_direction };
}

Vec3 Camera::sample_square() const noexcept {
    return Vec3{ rt::random_double() - 0.5, rt::random_double() - 0.5, 0 };
}

Point3 Camera::defocus_disk_sample() const noexcept {
    const Vec3 p{ random_in_unit_disk() };
    return center + (defocus_disk_u * p.x()) + (defocus_disk_v * p.y());
}