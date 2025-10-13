#include <string>
#include <thread>
#include <algorithm>

#include "camera.h"
#include "hittable_list.h"
#include "renderer.h"

int main(int argc, char* argv[]) {
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

    // Renderer

    CameraProperties cam_prop;
    // if (DEBUG) -> these
    cam_prop.samples_per_pixel = 10;
    cam_prop.max_depth         = 10;

    Camera cam{ cam_prop };
    HittableList world;

    Renderer renderer{ cam, world, filename, num_threads };
    renderer.render();

    return 0;
}
