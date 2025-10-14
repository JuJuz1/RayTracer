
#include "ray.h"

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

constexpr double eps{ 1e-6 };
using Catch::Matchers::WithinAbs;

TEST_CASE("At returns correct point along the direction", "[ray]") {
    const auto [ray, param, exp]{ GENERATE(table<Ray, double, Point3>({
        { Ray{ { 1, 6.1, 2.1 }, { -1, 2, 5} },
            2.5, { -1.5, 11.1, 14.6 } },
        { Ray{ { -2.1, 9.8, 0 }, { 6.61, 0, 5.9} },
            -9.5, { -64.895, 9.8, -56.05 } },
        { Ray{ { 0, -11.2, -0.1 }, { -7.21, -0.5, 0.9} },
            0.515, { -3.71315, -11.4575, 0.3635 } }
    }))};

    const Point3 res{ ray.at(param) };
    REQUIRE_THAT(res.x(), WithinAbs(exp.x(), eps));
    REQUIRE_THAT(res.y(), WithinAbs(exp.y(), eps));
    REQUIRE_THAT(res.z(), WithinAbs(exp.z(), eps));
}
