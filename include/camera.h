#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

#include <string>
#include <vector>
#include <thread>

#include "vec3.h"
#include "hittable.h"
#include "color.h"
#include "ray.h"

class Camera {
 public:
    double aspect_ratio    = 1.0; // Ratio of image width over height
    int image_width        = 100; // Rendered image width
    int samples_per_pixel  =  10; // Count of random samples per pixel
    int max_depth          =  10; // Maximum ray bounces (recursion calls)
    double ray_attenuation = 0.1; // Fraction of light the ray preserves per bounce

    double vfov         = 90.0;               // Vertical field of view
    Point3 lookfrom     = Point3{ 0, 0,  0 }; // The point the camera is looking from
    Point3 lookat       = Point3{ 0, 0, -1 }; // -||- looking at
    Vec3 vup            = Point3{ 0, 1,  0 }; // Camera-relative "up" direction

    double defocus_angle =    0; // Variation angle of rays through each pixel
    double focus_dist    = 10.0; // Distance from lookfrom point to plane of perfect focus

    Color background_color_top    = Colors::LightBlue; // Gradient start color (top)
    Color background_color_bottom = Colors::White;     // -||- end

    // Use Hittable so we can also use HittableList
    bool Camera::render(
        const Hittable& world,
        const std::string& filename,
        std::vector<std::thread>& threads,
        uint32_t num_threads) noexcept;

 private:
    int image_height;          // Rendered image height
    double pixel_sample_scale; // Color scale factor for a sum of pixel samples
    Point3 center;             // Camera center
    Point3 pixel00_loc;        // Location of pixel (0, 0)
    Vec3 pixel_delta_u;        // Horizontal offset of a pixel
    Vec3 pixel_delta_v;        // Vertical -||-
    Vec3 u, v, w;              // Camera frame basis vectors
    Vec3 defocus_disk_u;       // Defocus disk horizontal radius
    Vec3 defocus_disk_v;       // -||- vertical

    // Called at the start of render
    void initialize() noexcept;

    // Single-threaded
    void render_single_thread(const Hittable& world, std::ofstream& out) const noexcept;

    // Multithreaded
    void Camera::render_chunk_threaded(
        uint32_t j_start,
        uint32_t j_end,
        uint32_t i_end,
        const Hittable& world,
        Color* buffer) const noexcept;

    void print_properties() const noexcept;

    // Calculates the color of a pixel with a given ray from the camera
    // Takes into account the passed Hittable object(s)
    Color trace_ray(const Ray& r, int depth, const Hittable& world) const noexcept;

    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j
    Ray get_ray(int i, int j) const noexcept;

    // Returns a vector to a random point in the [-0.5,-0.5]-[+0.5,+0.5] unit square
    Vec3 sample_square() const noexcept;

    // Returns a random point in the defocus disk
    Point3 defocus_disk_sample() const noexcept;
};

#endif // INCLUDE_CAMERA_H_
