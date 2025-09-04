#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"

class Camera {
    public:
        double aspect_ratio = 1.0; // Ratio of image over height
        int image_width = 100;     // Rendered image width
        
        // Use Hittable so now we can also use Hittable_list
        void render(const Hittable& world);

    private:
        int image_height;   // Rendered image height
        Point3 center;      // Camera center
        Point3 pixel00_loc; // Location of pixel (0, 0)
        Vec3 pixel_delta_u; // Horizontal offset of a pixel
        Vec3 pixel_delta_v; // Vertical -||-

        // Called at the start of render() to initialize
        // private variables according to the public ones
        void initialize();

        // Calculates the color of a pixel with a given ray from the camera
        // Takes into account the passed Hittable object(s)
        color ray_color(const Ray& r, const Hittable& world) const;
};

#endif