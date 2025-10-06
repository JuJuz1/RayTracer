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

    // Generate lots of small random spheres
    // The amount is: x <= (sphere_position_edge * 2)^2
    // as we discard those which overlap with the big spheres
    constexpr int sphere_position_edge{ 11 }; // 11
    constexpr double sphere_radius{ 0.2 };

    std::vector<Material> materials;
    // Reserve the same amount as max spheres with this setup
    materials.reserve(487);

    Material mat_ground{ Material{ MaterialType::Lambertian, new LambertianData{ Colors::Gray }} };
    materials.push_back(mat_ground);
    world.add_sphere(Point3{  0.0, -1000.0, -1.0 }, 1000.0, mat_ground);

    Material mat_glass{ Material{ MaterialType::Dielectric, new DielectricData{ refraction_indeces::Glass }} };
    materials.push_back(mat_glass);


    for (int a{ -sphere_position_edge }; a < sphere_position_edge; ++a) {
        for (int b{ -sphere_position_edge }; b < sphere_position_edge; ++b) {
            const double choose_mat{ rt::random_double() };
            const Point3 sphere_position{ a + rt::random_double() * 0.9, 0.2, b + rt::random_double() * 0.9};

            // Check for overlap with big spheres
            if ((sphere_position - Point3{ 4, sphere_radius, 0 }).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // Diffuse
                    const Color albedo{ random_vector() * random_vector() };
                    materials.push_back(Material{ MaterialType::Lambertian, new LambertianData{ albedo } });
                    world.add_sphere(sphere_position, sphere_radius, materials.back());
                } else if (choose_mat < 0.95) {
                    // Metal
                    const Color albedo{ random_vector(0.5, 1.0) };
                    const double fuzz{ rt::random_double(0, 0.5) };
                    materials.push_back(Material{ MaterialType::Metal, new MetalData{ albedo, fuzz } });
                    world.add_sphere(sphere_position, sphere_radius, materials.back());
                } else {
                    // Glass
                    world.add_sphere(sphere_position, sphere_radius, mat_glass);
                }
            }
        }
    }

    // Big spheres left to right (furthest to closest)
    Material mat_diffuse{ Material{ MaterialType::Lambertian, new LambertianData{ Colors::Orange }} };
    materials.push_back(mat_diffuse);
    world.add_sphere(Point3{ -4, 1, 0 }, 1.0, mat_diffuse);

    world.add_sphere(Point3{ 0, 1, 0 }, 1.0, mat_glass);

    Material mat_metal{ Material{ MaterialType::Metal, new MetalData{ Colors::Brown, 0.0 }} };
    materials.push_back(mat_metal);
    world.add_sphere(Point3{ 4, 1, 0 }, 1.0, mat_metal);

    // Camera

    Camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    // For fast debug renders use:
    // - samples_per_pixel 10
    // - max_depth 10
    cam.image_width       = 400; // original: 400
    cam.samples_per_pixel = 100; // original: 100
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

    if (!cam.render(world, filename, num_threads))
        return 0;

    const int objects{ world.count() };
    std::cout << "\nMaterials created: " << materials.size() << "\n";
    std::cout << "Spheres created: " << objects << " (3 big ones)\n";

    // Cleanup materials
    for (auto& mat : materials) {
        switch (mat.type) {
            case MaterialType::Lambertian:
                delete static_cast<LambertianData*>(mat.data);
                break;
            case MaterialType::Metal:
                delete static_cast<MetalData*>(mat.data);
                break;
            case MaterialType::Dielectric:
                delete static_cast<DielectricData*>(mat.data);
                break;
            default: break;
        }
    }

    materials.clear();

    return 0;
}
