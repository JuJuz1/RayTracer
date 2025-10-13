#include <memory>
#include <ranges>
#include <string>
#include <thread>
#include <algorithm>
#include <iostream>

#include "hittable_list.h"
#include "material.h"
#include "vec3.h"
#include "color.h"
#include "rtweekend.h"
#include "sphere.h"
#include "camera.h"

int main(int argc, char* argv[]) {
    using std::make_shared;
    using std::make_unique;

    // Contains every hittable object
    HittableList world;

    const auto mat_ground{ make_shared<Lambertian>(colors::Gray) };
    world.add(make_unique<Sphere>(Point3{  0.0, -1000.0, -1.0 }, 1000.0, mat_ground));

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
                    const double fuzz{ rt::random_double(0, 0.5) };
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
    world.add(make_unique<Sphere>(Point3{ -4, 1, 0 }, 1.0, mat_diffuse));

    world.add(make_unique<Sphere>(Point3{ 0, 1, 0 }, 1.0, mat_glass));

    const auto mat_metal{ make_shared<Metal>(colors::Brown, 0.0) };
    world.add(make_unique<Sphere>(Point3{ 4, 1, 0 }, 1.0, mat_metal));

    // Camera

    Camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    // For fast debug renders use e.g.
    // - samples_per_pixel 10
    // - max_depth 10
    cam.image_width       = 400; // original: 400
    cam.samples_per_pixel =  10; // original: 100
    cam.max_depth         =  10; // original: 50
    // Final image properties:
    // 1200, 500, 50

    // Viewport
    cam.vfov     = 20.0;
    cam.lookfrom = Point3{ 13, 2, 3 };
    cam.lookat   = Point3{  0, 0, 0 };
    cam.vup      = Vec3  {  0, 1, 0 };

    // Lens
    cam.defocus_angle =  0.6;
    cam.focus_dist    = 10.0;

    // Arguments

    int num_threads{ 1 };
    std::string filename{ "image.ppm" };

    if (1 < argc) {
        num_threads = std::stoi(argv[1]);
    }
    if (argc == 3) {
        filename = argv[2];
    }

    const int max_threads = static_cast<int>(std::thread::hardware_concurrency());
    num_threads = std::min(num_threads, max_threads);

    if (!cam.render(world, filename, num_threads)) [[unlikely]]
        return 0;

    const int objects{ world.count() };
    std::cout << "\nSpheres created: " << objects << " (3 big ones)\n";

    return 0;
}
