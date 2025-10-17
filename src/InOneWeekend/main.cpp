#include <string_view>
#include <string>
#include <thread>
#include <algorithm>
#include <utility>

#include "camera.h"
#include "hittable_list.h"
#include "renderer.h"

// Parses command line arguments for the program
// Argv's strings exist for the lifetime of the program
// -> using string_view here is fine
auto parse_arguments(int argc, char* argv[]) {
    // Default values
    int num_threads{ 1 };
    std::string_view filename{ "image.ppm" };

    if (1 < argc) {
        num_threads = std::stoi(argv[1]);
    }
    if (argc == 3) {
        filename = argv[2];
    }

    const int max_threads = static_cast<int>(std::thread::hardware_concurrency());
    num_threads = std::min(num_threads, max_threads);

    return std::pair(num_threads, filename);
}

int main(int argc, char* argv[]) {
    const auto [num_threads, filename]{ parse_arguments(argc, argv) };

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
