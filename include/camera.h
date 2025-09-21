#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"

class Camera {
    public:
        double aspect_ratio    = 1.0; // Ratio of image width over height
        int image_width        = 100; // Rendered image width
        int samples_per_pixel  = 10;  // Count of random samples per pixel
        int max_depth          = 10;  // Maximum ray bounces (recursion calls)
        double ray_attenuation = 0.1; // Fraction of light the ray preserves per bounce
        
        double vfov         = 90;                 // Vertical field of view
        Point3 lookfrom     = Point3{ 0, 0, 0 };  // The point the camera is looking from
        Point3 lookat       = Point3{ 0, 0, -1 }; // -||- looking at
        Vec3 vup            = Point3{ 0, 1, 0 };  // Camera-relative "up" direction
        
        // Use Hittable so we can also use Hittable_list
        void render(const Hittable& world) noexcept;

    private:
        int image_height;          // Rendered image height
        double pixel_sample_scale; // Color scale factor for a sum of pixel samples
        Point3 center;             // Camera center
        Point3 pixel00_loc;        // Location of pixel (0, 0)
        Vec3 pixel_delta_u;        // Horizontal offset of a pixel
        Vec3 pixel_delta_v;        // Vertical -||-
        Vec3 u, v, w;              // Camera frame basis vectors

        // Called at the start of render() to initialize private variables
        void initialize() noexcept;

        void print_properties() const noexcept;

        // Calculates the color of a pixel with a given ray from the camera
        // Takes into account the passed Hittable object(s)
        color ray_color(const Ray& r, int depth, const Hittable& world) const noexcept;

        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j
        Ray get_ray(int i, int j) const noexcept;

        // Returns the vector to a random point in the [-0.5,-0.5]-[+0.5,+0.5] unit square
        Vec3 sample_square() const noexcept;
};

#endif