#include "vec3.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"

int main() {

    // World

    // Contains every hittable object
    Hittable_list world;
    // This is very strange, for example radius of 0.88 makes the sphere almost 2 times smaller
    // Values below 0.8 don't even produce the sphere at all...
    world.add(std::make_unique<Sphere>(Point3{ 0, 0, -1.0 }, 0.9));
    world.add(std::make_unique<Sphere>(Point3{ 0, -100.5, -1 }, 100));

    // Camera

    Camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400; 
    cam.samples_per_pixel = 100; 
    cam.max_depth         = 50;  
    cam.ray_attenuation   = 0.3; // 30% seems good with gamma correction

    cam.render(world);

    return 0;
}