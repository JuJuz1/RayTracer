#include "renderer.h"

#include <string_view>
#include <utility>

#include <fstream>
#include <iostream>

#include <memory>
#include <ranges>

#include <vector>
#include <thread>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "rtweekend.h"
#include "timer.h"

Renderer::Renderer(
    const Camera& cam,
    HittableList& world,
    std::string_view output_filename,
    int num_threads
) noexcept : cam{ cam },
    world{ std::move(world) }, output_filename{ output_filename }, num_threads{ num_threads } {
    populate_world();
}

void Renderer::render() const {
    cam.print_properties();

    // Open for output and clear existing content
    std::ofstream out{ output_filename.data() };
    if (!out.is_open()) {
        std::cerr << "Error while opening file!\n";
        return;
    }

    std::cout << "Rendering output to file: " << output_filename << "\n";

    const int image_width{ cam.get_image_width() };
    const int image_height{ cam.get_image_height() };
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Single-threaded
    if (num_threads == 1) {
        std::cout << "\nSingle thread\n";
        cam.render_single_thread(world, out);
        out.close();
        return;
    }

    // Multithreaded
    std::cout << "\nMultithread\n";
    std::cout << "Thread count: " << num_threads << "\n";

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // Color buffer for threads to store colors to
    std::vector<Color> color_buffer;
    color_buffer.resize(image_width * image_height);

    setup_threads(threads, color_buffer, image_width, image_height);

    Timer t;
    // Execute threads
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Writing to file...\n";

    write_color_from_buffer(out, color_buffer);

    t.print_elapsed("Total time: ");

    out.close();
    return;
}

// === Private ===

void Renderer::setup_threads(
    std::vector<std::thread>& threads,
    std::vector<Color>& color_buffer,
    int image_width,
    int image_height
) const {
    const int rows_per_thread{ image_height / num_threads };
    const int leftover{ image_height % num_threads };

    // Assign each thread a range of rows that it writes to
    for (int n : std::views::iota(0, num_threads)) {
        const int j_start{ n * rows_per_thread };
        int j_end{ j_start + rows_per_thread };
        // Add all leftover to last thread
        if (n == num_threads - 1)
            j_end += leftover;

        threads.emplace_back(
            &Camera::render_chunk,
            &cam,
            j_start,
            j_end,
            std::cref(world), // Const ref
            std::ref(color_buffer));
    }
}

void Renderer::populate_world() {
    using std::make_shared;
    using std::make_unique;

    const auto mat_ground{ make_shared<Lambertian>(colors::Gray) };
    world.add(make_unique<Sphere>(Point3{ 0.0, -1000.0, -1.0 }, 1000.0, mat_ground));

    const auto mat_glass{ make_shared<Dielectric>(refraction_indeces::Glass) };

    // Generate lots of small random spheres
    // The amount is: x <= (sphere_position_edge * 2)^2
    // as we discard those which overlap with the big spheres
    constexpr int sphere_position_edge{ 11 }; // 11
    constexpr double sphere_radius{ 0.2 };
    constexpr double multiplier{ 0.9 };

    for (int a : std::views::iota(-sphere_position_edge, sphere_position_edge)) {
        for (int b : std::views::iota(-sphere_position_edge, sphere_position_edge)) {
            const double choose_mat{ rt::random_double() };
            const Point3 sphere_position{
                a + rt::random_double() * multiplier,
                sphere_radius,
                b + rt::random_double() * multiplier};

            // Check for overlap with big spheres
            if (multiplier < (sphere_position - Point3{ 4, sphere_radius, 0 }).length()) [[likely]] {
                std::shared_ptr<Material> mat;

                if (choose_mat < 0.8) [[likely]] {
                    // Diffuse
                    const Color albedo{ random_vector() * random_vector() };
                    mat = make_shared<Lambertian>(albedo);
                    world.add(make_unique<Sphere>(sphere_position, sphere_radius, mat));
                } else if (choose_mat < 0.95) [[unlikely]] {
                    // Metal
                    const Color albedo{ random_vector(0.5, 1.0) };
                    const double fuzz{ rt::random_double(0.0, 0.5) };
                    mat = make_shared<Metal>(albedo, fuzz);
                    world.add(make_unique<Sphere>(sphere_position, sphere_radius, mat));
                } else [[unlikely]] {
                    // Glass
                    world.add(make_unique<Sphere>(sphere_position, sphere_radius, mat_glass));
                }
            }
        }
    }

    // Big spheres left to right (furthest to closest)
    const auto mat_diffuse{ make_shared<Lambertian>(colors::Orange) };
    world.add(make_unique<Sphere>(Point3{ -4.0, 1.0, 0.0 }, 1.0, mat_diffuse));

    world.add(make_unique<Sphere>(Point3{ 0.0, 1.0, 0.0 }, 1.0, mat_glass));

    const auto mat_metal{ make_shared<Metal>(colors::Brown, 0.0) };
    world.add(make_unique<Sphere>(Point3{ 4.0, 1.0, 0.0 }, 1.0, mat_metal));

    const int objects{ world.count() };
    std::cout << "\nSpheres created: " << objects << " (3 big ones)\n";
}
