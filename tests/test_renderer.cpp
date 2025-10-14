#include "renderer.h"

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <format>

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"

/*
Helper functions to mute console out when tests are run
A bit of a dirty trick but gets the job done without modifying source code at least
*/

// Mutes cout
std::streambuf* mute_cout() {
    std::streambuf* old_buf = std::cout.rdbuf();
    // Static null_stream that std::cout points to
    static std::ostringstream null_stream;
    // redirect cout to nowhere
    std::cout.rdbuf(null_stream.rdbuf());

    return old_buf;
}

// Restores original cout buffer
void remove_mute_cout(std::streambuf* old_buf) {
    std::cout.rdbuf(old_buf);
}

struct RendererFixture {
    std::string filename{ "test_image.ppm" };

    ~RendererFixture() {
        std::remove(filename.c_str());
    }
};

TEST_CASE_METHOD(RendererFixture, "File line count is correct after writing to file", "[renderer]") {
    const auto old_buf{ mute_cout() };

    const auto [image_width, expected_lines]{ GENERATE(table<int, int>({
        {20, 223}, {50, 1403}
    }))};

    CameraProperties cam_prop;
    cam_prop.image_width = image_width;

    Camera cam{ cam_prop };
    HittableList world;

    Renderer renderer{ cam, world, filename, 1 };
    renderer.render();

    remove_mute_cout(old_buf);

    // Check file is open, i.e. it was created
    std::ifstream f{ filename };
    REQUIRE(f.is_open());

    // Count lines
    int lines{ 0 };
    std::string line;
    while (std::getline(f, line))
        ++lines;

    REQUIRE(lines == expected_lines);
}


TEST_CASE_METHOD(RendererFixture, "File is correct PPM P3 format after writing to file", "[renderer]") {
    const auto old_buf{ mute_cout() };

    const int image_width{ GENERATE(5, 25) };

    CameraProperties cam_prop;
    cam_prop.image_width = image_width;

    Camera cam{ cam_prop };
    HittableList world;

    Renderer renderer{ cam, world, filename, 1 };
    renderer.render();

    remove_mute_cout(old_buf);

    std::ifstream f{ filename };
    REQUIRE(f.is_open());

    std::string line;
    // Check first few lines
    std::getline(f, line);
    // P3 format
    REQUIRE(line == "P3");

    const int image_height = static_cast<int>(image_width / cam_prop.aspect_ratio );
    std::getline(f, line);
    // image_width image_height
    REQUIRE(line == std::format("{} {}", image_width, image_height));

    std::getline(f, line);
    // Color range [0, 255]
    REQUIRE(line == "255");
}
