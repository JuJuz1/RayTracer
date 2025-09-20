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
    
    const auto mat_ground = make_shared<Lambertian>(Color::Lime);
    const auto mat_center = make_shared<Lambertian>(Color::DarkBlue);
    const auto mat_left   = make_shared<Metal>(Color::LightGray);
    const auto mat_right  = make_shared<Metal>(Color::Orange);

    // World

    // Contains every hittable object
    Hittable_list world;
    world.add(make_unique<Sphere>(Point3{  0.0, -100.5, -1.0 }, 100.0, mat_ground));
    world.add(make_unique<Sphere>(Point3{ -1.0,    0.0, -1.0 },   0.5, mat_left));
    world.add(make_unique<Sphere>(Point3{  0.0,    0.0, -1.3 },   0.5, mat_center));
    world.add(make_unique<Sphere>(Point3{  1.0,    0.3, -1.0 },   0.5, mat_right));

    // Camera

    Camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    // Material attenuation overrides Camera::ray_attenuation now
    //cam.ray_attenuation   = 0.3; // 30% seems good with gamma correction

    cam.render(world);

    return 0;
}