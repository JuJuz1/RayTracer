#include "vec3.h"

#include <vector>

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

constexpr double eps{ 1e-6 };
using Catch::Matchers::WithinAbs;

// A helper struct to contain fixture results
struct Vec3Fixture {
    Vec3 a, b;
    Vec3 expected;
};

TEST_CASE("Vec3 addition works correctly", "[vec3]") {
    std::vector<Vec3Fixture> examples{
        {{1.0, 2.0, 11.51}, {2.0, 0.87, -2.1}, {3.0, 2.87, 9.41}},
        {{-0.0, -1.231, 0.1}, {0.0, 0.912, -1.2}, {0.0, -0.319, -1.1}}
    };

    for (const auto& ex : examples) {
        Vec3 res{ ex.a + ex.b };
        REQUIRE_THAT(res.x(), WithinAbs(ex.expected.x(), eps));
        REQUIRE_THAT(res.y(), WithinAbs(ex.expected.y(), eps));
        REQUIRE_THAT(res.z(), WithinAbs(ex.expected.z(), eps));
    }
}

TEST_CASE("Vec3 subtraction works correctly", "[vec3]") {
    Vec3 a{ -1.0, 2.5, 2.5 };
    Vec3 b{ 5.23, -0.9, 0.7 };
    Vec3 c{ a - b };

    REQUIRE_THAT(c.x(), WithinAbs(-6.23, eps));
    REQUIRE_THAT(c.y(), WithinAbs(3.4, eps));
    REQUIRE_THAT(c.z(), WithinAbs(1.8, eps));
}
