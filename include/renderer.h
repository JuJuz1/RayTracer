#ifndef INCLUDE_RENDERER_H_
#define INCLUDE_RENDERER_H_

#include <string>
#include <vector>
#include <thread>

#include "camera.h"
#include "hittable_list.h"
#include "color.h"

class Renderer {
 public:
    Renderer(
        Camera& cam,
        HittableList& world,
        const std::string& output_filename,
        int num_threads) noexcept;

    // Handles rendering and writing to the file
    void render() const;

 private:
    Camera cam;
    HittableList world;
    std::string output_filename;
    int num_threads;

    // Populates the world with spheres
    void populate_world();

    // Setups threads and emplaces them in threads
    void setup_threads(
        std::vector<std::thread>& threads,
        std::vector<Color>& color_buffer,
        int image_width,
        int image_height) const;
};

#endif // INCLUDE_RENDERER_H_
