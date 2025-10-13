#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

#include <string>
#include <vector>
#include <thread>

#include "vec3.h"
#include "hittable_list.h"
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
    Point3 lookat       = Point3{ 0, 0, -1 }; // The point the camera is looking at
    Vec3 vup            = Point3{ 0, 1,  0 }; // Camera-relative "up" direction

    double defocus_angle =    0; // Variation angle of rays through each pixel
    double focus_dist    = 10.0; // Distance from lookfrom point to plane of perfect focus

    Color background_color_top    = colors::LightBlue; // Gradient start color (top)
    Color background_color_bottom = colors::White;     // Gradient end color (bottom)

    // The main character, handles single and multithreaded rendering
    [[nodiscard]] bool render(
        const HittableList& world,
        const std::string& filename,
        int num_threads) noexcept;

 private:
    int image_height;          // Rendered image height
    double pixel_sample_scale; // Color scale factor for a sum of pixel samples
    Point3 center;             // Camera center
    Point3 pixel00_loc;        // Location of pixel (0, 0)
    Vec3 pixel_delta_u;        // Horizontal offset of a pixel
    Vec3 pixel_delta_v;        // Vertical offset of a pixel
    Vec3 u, v, w;              // Camera frame basis vectors
    Vec3 defocus_disk_u;       // Defocus disk horizontal radius
    Vec3 defocus_disk_v;       // Defocus disk vertical radius

    // Called at the start of render
    void initialize() noexcept;

    // Single-threaded
    void render_single_thread(const HittableList& world, std::ofstream& out) const noexcept;

    // Multithreaded
    void render_chunk_multithreaded(
        int j_start,
        int j_end,
        int i_end,
        const HittableList& world,
        std::vector<Color>& color_buffer) const noexcept;

    void print_properties() const noexcept;

    // Calculates the color of a pixel with a given ray from the camera
    // Takes into account the passed Hittable object(s)
    [[nodiscard]] Color trace_ray(const Ray& r, int depth, const HittableList& world) const noexcept;

    // Constructs a camera ray originating from the origin
    // and directed at a randomly sampled point around the pixel location i, j
    [[nodiscard]] Ray get_ray(int i, int j) const noexcept;

    // Returns a random point in the defocus disk
    [[nodiscard]] Point3 defocus_disk_sample() const noexcept;
};

// Returns a vector to a random point in the [-0.5,-0.5]-[+0.5,+0.5] unit square
[[nodiscard]] Vec3 sample_square() noexcept;

#endif // INCLUDE_CAMERA_H_
