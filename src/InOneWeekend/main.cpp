#include <memory>
#include <cmath>
#include <iostream>

#include <vector>
#include <thread>

#include <string>
#include <algorithm>

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

    const auto mat_ground{ make_shared<Lambertian>(Colors::Gray) };
    world.add(make_unique<Sphere>(Point3{  0.0, -1000.0, -1.0 }, 1000.0, mat_ground));

    const auto mat_glass{ make_shared<Dielectric>(refraction_indeces::Glass) };

    // Generate lots of small random spheres
    // The amount is: x <= (sphere_position_edge * 2)^2
    // as we discard those which overlap with the big spheres
    constexpr int sphere_position_edge{ 11 }; // 11
    constexpr double sphere_radius{ 0.2 };
    for (int a{ -sphere_position_edge }; a < sphere_position_edge; ++a) {
        for (int b{ -sphere_position_edge }; b < sphere_position_edge; ++b) {
            const double choose_mat{ rt::random_double() };
            const Point3 sphere_position{
                a + rt::random_double() * 0.9, 0.2, b + rt::random_double() * 0.9};

            // Check for overlap with big spheres
            if ((sphere_position - Point3{ 4, sphere_radius, 0 }).length() > 0.9) {
                std::shared_ptr<Material> mat;

                if (choose_mat < 0.8) {
                    // Diffuse
                    const Color albedo{ random_vector() * random_vector() };
                    mat = make_shared<Lambertian>(albedo);
                    world.add(make_unique<Sphere>(sphere_position, sphere_radius, mat));
                } else if (choose_mat < 0.95) {
                    // Metal
                    const Color albedo{ random_vector(0.5, 1.0) };
                    const double fuzz{ rt::random_double(0, 0.5) };
                    mat = make_shared<Metal>(albedo, fuzz);
                    world.add(make_unique<Sphere>(sphere_position, sphere_radius, mat));
                } else {
                    // Glass
                    world.add(make_unique<Sphere>(sphere_position, sphere_radius, mat_glass));
                }
            }
        }
    }

    // Big spheres left to right (furthest to closest)
    const auto mat_diffuse{ make_shared<Lambertian>(Colors::Orange) };
    world.add(make_unique<Sphere>(Point3{ -4, 1, 0 }, 1.0, mat_diffuse));

    world.add(make_unique<Sphere>(Point3{ 0, 1, 0 }, 1.0, mat_glass));

    const auto mat_metal{ make_shared<Metal>(Colors::Brown, 0.0) };
    world.add(make_unique<Sphere>(Point3{ 4, 1, 0 }, 1.0, mat_metal));

    // Camera
    Camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    // For fast debug renders use:
    // - samples_per_pixel 10
    // - max_depth 10
    cam.image_width       = 400; // original: 400
    cam.samples_per_pixel = 10; // original: 100
    cam.max_depth         =  50; // original: 50
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

    std::string filename{ "image.ppm" };
    uint32_t num_threads{ 1 };
    if (argc == 2) {
        num_threads = std::stoi(argv[1]);
    } else if (argc == 3) {
        num_threads = std::stoi(argv[1]);
        filename = argv[2];
    }

    std::vector<std::thread> threads;
    const uint32_t max_threads{ std::thread::hardware_concurrency() };
    num_threads = std::min(num_threads, max_threads);

    if (!cam.render(world, filename, threads, num_threads))
        return 0;

    const int objects{ world.count() };
    std::cout << "\nSpheres created: " << objects << " (3 big ones)\n";

    return 0;
}
