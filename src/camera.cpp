#include "camera.h"

#include <mutex>
#include <atomic>

#include <string>
#include <vector>
#include <thread>

#include <chrono>
#include <iostream>
#include <fstream>

#include <functional>
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
//std::thread::id progress_thread_id;
std::atomic<int> scanlines_done{ 0 };

bool Camera::render(
    const Hittable& world,
    const std::string& filename,
    std::vector<std::thread>& threads,
    uint32_t num_threads
) noexcept {
    initialize();

    // Open for output and clear existing content
    std::ofstream out{ filename };
    if (!out.is_open()) {
        std::cerr << "Error while opening file!\n";
        return false;
    }

    std::cout << "\nRendering output to file: " << filename << "\n";
    print_properties();

    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Single-threaded
    if (num_threads == 1) {
        std::cout << "Single thread\n";
        render_single_thread(world, out);
        out.close();
        return true;
    }

    // Multithreaded
    std::cout << "Multithread\n";
    std::cout << "Thread count: " << num_threads << "\n";

    threads.reserve(num_threads);

    const uint32_t rows_per_thread{ image_height / num_threads };
    const uint32_t leftover{ image_height % num_threads };

    // Buffers for threads
    std::vector<Color> color_buffer;
    color_buffer.reserve(image_width * image_height);

    for (uint32_t n{ 0 }; n < num_threads; ++n) {
        const uint32_t j_start{ n * rows_per_thread };
        uint32_t j_end{ j_start + rows_per_thread };
        // Add all leftover to last thread
        if (n == num_threads - 1)
            j_end += leftover;

        threads.emplace_back(
            &Camera::render_chunk_threaded,
            this,
            j_start,
            j_end,
            static_cast<uint32_t>(image_width),
            std::cref(world), // Const ref
            std::ref(color_buffer));
    }

    // Progress indicators
    Timer t;
    //progress_thread_id = threads.front().get_id();

    // Execute threads
    for (auto& th : threads) {
        th.join();
        //if (th.get_id() == threads.front().get_id())
    }

    std::cout << "Writing to file...\n";

    for (uint32_t n{ 0 }; n < num_threads; ++n) {
        write_color(out, color_buffer);
    }

    t.print_elapsed("Total time: ");

    out.close();
    return true;
}

void Camera::render_single_thread(const Hittable& world, std::ofstream& out) const noexcept {
    Timer t;
    double last_print{ t.elapsed() };
    constexpr double progress_refresh_rate{ 0.5 };

    for (int j{ 0 }; j < image_height; ++j) {
        for (int i{ 0 }; i < image_width; ++i) {
            Color pixel_color{};
            for (int sample{ 0 }; sample < samples_per_pixel; ++sample) {
                const Ray r{ get_ray(i, j) };
                pixel_color += trace_ray(r, max_depth, world);
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

void Camera::render_chunk_threaded(
    uint32_t j_start,
    uint32_t j_end,
    uint32_t i_end,
    const Hittable& world,
    std::vector<Color>& color_buffer
) const noexcept {
    // Released after scope
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread " << std::this_thread::get_id()
                  << " rows: " << j_start << " to " << j_end << "\n";
    }

    Timer t;

    for (uint32_t j{ j_start }; j < j_end; ++j) {
        for (uint32_t i{ 0 }; i < i_end; ++i) {
            Color pixel_color;
            for (int sample{ 0 }; sample < samples_per_pixel; ++sample) {
                // Row as in viewport
                const Ray r{ get_ray(i, j) };
                pixel_color += trace_ray(r, max_depth, world);
            }

            color_buffer[j * i_end + i] = pixel_color * pixel_sample_scale;
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
