#include "camera.h"

#include <mutex>
#include <atomic>

#include <string>
#include <fstream>
#include <iostream>

#include <vector>
#include <thread>
#include <functional>

#include <ranges>
#include <algorithm>
#include <cmath>

#include "color.h"
#include "hittable.h"
#include "timer.h"
#include "rtweekend.h"
#include "material.h"

// Thread-safe console out
std::mutex cout_mutex;

// Progress indicator for multithreading
std::atomic<int> scanlines_done{ 0 };

Camera::Camera(const CameraProperties& cam_prop) : cam_prop{ cam_prop } {
    initialize();
}

int Camera::get_image_width() const noexcept {
    return cam_prop.image_width;
}

int Camera::get_image_height() const noexcept {
    return image_height;
}

void Camera::print_properties() const noexcept {
    std::cout << "\nRendering an image with properties:\n";
    rt::print_camera_property_formatted("Width", cam_prop.image_width);
    rt::print_camera_property_formatted("Height", image_height);
    rt::print_camera_property_formatted("Samples per pixel", cam_prop.samples_per_pixel);
    rt::print_camera_property_formatted("Max depth", cam_prop.max_depth);

    std::cout << "\nViewport properties:\n";
    rt::print_camera_property_formatted("Vertical fov", cam_prop.viewport_prop.vfov);
    rt::print_camera_property_formatted("Look from", cam_prop.viewport_prop.lookfrom);
    rt::print_camera_property_formatted("Look at", cam_prop.viewport_prop.lookat);

    std::cout << "\nLens properties:\n";
    rt::print_camera_property_formatted("Defocus angle", cam_prop.lens_prop.defocus_angle);
    rt::print_camera_property_formatted("Focus distance", cam_prop.lens_prop.focus_dist);
    std::cout << "\n";
}

void Camera::render_single_thread(const HittableList& world, std::ofstream& out) const noexcept {
    Timer t;
    double last_print{ t.elapsed() };
    constexpr double progress_refresh_rate{ 0.5 };

    for (int j : std::views::iota(0, image_height)) {
        for (int i : std::views::iota(0, cam_prop.image_width)) {
            Color pixel_color;
            for ([[maybe_unused]] auto _ : std::views::iota(0, cam_prop.samples_per_pixel)) {
                const Ray r{ construct_ray(i, j) };
                pixel_color += trace_ray(r, cam_prop.max_depth, world);
            }

            write_color(out, pixel_color * pixel_sample_scale);
        }

        const double elapsed{ t.elapsed() };
        if (progress_refresh_rate < elapsed - last_print) {
            print_progress_single_thread(t, elapsed, j, image_height);
            last_print = elapsed;
        }
    }
}

void Camera::render_chunk(
    int j_start,
    int j_end,
    const HittableList& world,
    std::vector<Color>& color_buffer
) const noexcept {
    // Released after scope
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread " << std::this_thread::get_id()
                  << " rows: " << j_start << " to " << j_end << "\n";
    }

    Timer t;

    for (int j : std::views::iota(j_start, j_end)) {
        for (int i : std::views::iota(0, cam_prop.image_width)) {
            Color pixel_color;
            for ([[maybe_unused]] auto _ : std::views::iota(0, cam_prop.samples_per_pixel)) {
                const Ray r{ construct_ray(i, j) };
                pixel_color += trace_ray(r, cam_prop.max_depth, world);
            }

            color_buffer[j * cam_prop.image_width + i] = pixel_color * pixel_sample_scale;
        }

        ++scanlines_done;
        // TODO: maybe have a designated thread handle logging, no need for mutex
        // Update progress every 10 rows
        if (scanlines_done % 10 == 0) {
            const int done{ scanlines_done.load() };
            const double percent{ (done * 100.0) / image_height };
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "\rProgress: " << std::fixed << std::setprecision(1)
                        << percent << "% | Elapsed time: "
                        << std::setprecision(3) << t.elapsed() << "s" << std::flush;
        }
    }

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "\nThread " << std::this_thread::get_id()
                  << " finished in "
                  << std::fixed << std::setprecision(3) << t.elapsed() << "s" << "\n";
    }
}

// === Private ===

void Camera::initialize() noexcept {
    image_height = std::max(static_cast<int>(cam_prop.image_width / cam_prop.aspect_ratio), 1);

    pixel_sample_scale = 1.0 / cam_prop.samples_per_pixel;

    center = cam_prop.viewport_prop.lookfrom;

    // Viewport dimensions
    const double theta{ rt::degrees_to_radians(cam_prop.viewport_prop.vfov) };
    const double h{ std::tan(theta / 2) };
    const double viewport_height{ 2 * h * cam_prop.lens_prop.focus_dist };
    // Determine viewport_width from the actual image size, can't be perfect in terms of aspect_ratio
    const double viewport_width = viewport_height * (static_cast<double>(cam_prop.image_width) / image_height);

    // Camera unit basis vectors
    w = unit_vector(cam_prop.viewport_prop.lookfrom - cam_prop.viewport_prop.lookat);
    u = unit_vector(cross(cam_prop.viewport_prop.vup, w));
    v = cross(w, u);

    // Calculate the vectors for horizontal and vertical traversing of the viewport
    const Vec3 viewport_u{ u * viewport_width };
    const Vec3 viewport_v{ -v * viewport_height };

    // Horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / cam_prop.image_width;
    pixel_delta_v = viewport_v / image_height;

    const Point3 viewport_upper_left{
        center - (w * cam_prop.lens_prop.focus_dist) - viewport_u / 2 - viewport_v / 2 };
    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

    // Defocus disk basis vectors
    const double defocus_radius{cam_prop.lens_prop.focus_dist
        * std::tan(rt::degrees_to_radians(cam_prop.lens_prop.defocus_angle / 2))};
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

Color Camera::trace_ray(const Ray& r, int depth, const HittableList& world) const noexcept {
    // Hit ray bounce limit (max_depth)
    if (depth <= 0) [[unlikely]]
        return colors::Black;

    // If they ray's origin is just below the surface it might hit the surface immediately
    // An interval with min of 0.001 ignores hits that are very close
    if (auto hit_rec = world.process_ray(r, Interval{ 0.001, rt::infinity })) {
        if (auto scatter_rec = hit_rec->mat->scatter(r, *hit_rec))
            return scatter_rec->attenuation * trace_ray(scatter_rec->scattered, depth - 1, world);

        return colors::Black;
    }

    // Nothing was hit -> render background
    const Vec3 unit_direction{ unit_vector(r.direction()) };
    // Linear interpolation by scaling the y-coordinate to the range [0, 1]
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return cam_prop.bg_colors.background_color_bottom * (1.0 - a)
         + cam_prop.bg_colors.background_color_top * a;
}

Ray Camera::construct_ray(int i, int j) const noexcept {
    const Vec3 offset{ sample_square() };
    const Point3 pixel_sample{pixel00_loc
                           + (pixel_delta_u * (i + offset.x()))
                           + (pixel_delta_v * (j + offset.y()))};

    const Point3 ray_origin{ (cam_prop.lens_prop.defocus_angle <= 0) ? center : defocus_disk_sample() };
    const Vec3 ray_direction{ pixel_sample - ray_origin };
    return Ray{ ray_origin, ray_direction };
}

Point3 Camera::defocus_disk_sample() const noexcept {
    const Vec3 p{ random_in_unit_disk() };
    return center + (defocus_disk_u * p.x()) + (defocus_disk_v * p.y());
}

Vec3 sample_square() noexcept {
    return Vec3{ rt::random_double() - 0.5, rt::random_double() - 0.5, 0 };
}
