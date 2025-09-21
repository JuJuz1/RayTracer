#include <memory>

#include "material.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"

int main() {

    using std::make_shared;
    using std::make_unique;

    // Materials
    
    constexpr double glass_refraction_index{ 1.50 };
    const auto mat_ground = make_shared<Lambertian>(Color::Lime);
    const auto mat_center = make_shared<Lambertian>(Color::DarkBlue);
    //const auto mat_left   = make_shared<Metal>(Color::LightGray, 0.3);
    const auto mat_left   = make_shared<Dielectric>(glass_refraction_index);
    const auto mat_bubble = make_shared<Dielectric>(1.0 / glass_refraction_index);
    const auto mat_right  = make_shared<Metal>(Color::Orange, 0.8);

    // World

    // Contains every hittable object
    Hittable_list world;
    world.add(make_unique<Sphere>(Point3{  0.0, -100.5, -1.0 }, 100.0,  mat_ground));
    world.add(make_unique<Sphere>(Point3{  0.0,    0.0, -1.2 },   0.5,  mat_center));
    world.add(make_unique<Sphere>(Point3{ -1.0,    0.0, -1.0 },   0.5,  mat_left));
    world.add(make_unique<Sphere>(Point3{ -1.0,    0.0, -1.0 },   0.4,  mat_bubble));
    world.add(make_unique<Sphere>(Point3{  1.1,    0.0, -1.0 },   0.5, mat_right));

    // Camera

    Camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    // Viewport
    cam.vfov     = 90;
    cam.lookfrom = Point3{ -2, 2,  1 };
    cam.lookat   = Point3{  0, 0, -1 };
    cam.vup      = Vec3  {  0, 1,  0 };

    cam.render(world);

    return 0;
}