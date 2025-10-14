
#include "color.h"

#include <string>
#include <stdio.h>
#include <fstream>

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

constexpr double eps{ 1e-6 };
using Catch::Matchers::WithinAbs;

struct ColorFixture {
    std::string filename{ "test_output.ppm" };

    ~ColorFixture() {
        std::remove(filename.c_str());
    }
};

struct ColorData {
    Color input;
    int exp_r, exp_g, exp_b;
};

TEST_CASE("Color value to gamma and byte range produces correct result", "[color]") {
    const auto [a, exp]{ GENERATE(table<double, int>({
        {50, 255}, {0.7, 214}, {0.97, 252}, {1.001, 255}, {300, 255}, {-12.32, 0}
    }))};

    const int res{ to_gamma_and_byte_range(a) };
    REQUIRE_THAT(res, WithinAbs(exp, eps));
}

TEST_CASE_METHOD(ColorFixture, "Write color to stream writes correct color", "[color]") {
    const auto [data, exp_r, exp_g, exp_b]{ GENERATE(values<ColorData>({
        {{50.2, 0.12, 116.1}, 255, 88, 255},
        {{0.251, 0.81, 116.1}, 128, 230, 255},
        {{-2.1, 12.125, 0.515}, 0, 255, 183}
    }))};

    std::ofstream out{ filename };
    REQUIRE(out.is_open());

    // Output to file
    write_color_to_stream(out, data.x(), data.y(), data.z());
    out.close();

    std::ifstream f{ filename };
    REQUIRE(f.is_open());

    // Read from file
    std::string line;
    std::getline(f, line);
    REQUIRE(line == std::format("{} {} {}", exp_r, exp_g, exp_b));
}
