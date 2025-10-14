#include "vec3.h"

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

constexpr double eps{ 1e-6 };
using Catch::Matchers::WithinAbs;

TEST_CASE("Vec3 addition works correctly", "[vec3]") {
    const auto [a, b, exp]{ GENERATE(table<Vec3, Vec3, Vec3>({
        {{1.0, 2.0, 11.51}, {2.0, 0.87, -2.1}, {3.0, 2.87, 9.41}},
        {{-0.0, -1.231, 0.1}, {0.0, 0.912, -1.2}, {0.0, -0.319, -1.1}}
    }))};

    const Vec3 res{ a + b };
    REQUIRE_THAT(res.x(), WithinAbs(exp.x(), eps));
    REQUIRE_THAT(res.y(), WithinAbs(exp.y(), eps));
    REQUIRE_THAT(res.z(), WithinAbs(exp.z(), eps));
}

TEST_CASE("Vec3 subtraction works correctly", "[vec3]") {
    const auto [a, b, exp]{ GENERATE(table<Vec3, Vec3, Vec3>({
        {{1, 21.8, 3.1}, {2.2, 5.12, 8}, {-1.2, 16.68, -4.9}},
        {{-0.12, -0.2, 3.1}, {-2.1, 0.5, -9}, {1.98, -0.7, 12.1}}
    }))};

    const Vec3 res{ a - b };
    REQUIRE_THAT(res.x(), WithinAbs(exp.x(), eps));
    REQUIRE_THAT(res.y(), WithinAbs(exp.y(), eps));
    REQUIRE_THAT(res.z(), WithinAbs(exp.z(), eps));
}
