
#include "ray.h"

#include "vec3.h"

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

TEST_CASE("At returns correct point along the direction", "[ray]") {
    const auto [ray, param, exp]{ GENERATE(table<Ray, double, Point3>({
        {Ray{ Point3{ 1, 6.1, 2.1 }, Point3{ -1, 2, 5} },
            2.5, Point3{ -1.5, 11.1, 14.6 }},
        {Ray{ Point3{ -2.1, 9.8, 0 }, Point3{ 6.61, 0, 5.9} },
            -9.5, Point3{ -64.895, 9.8, -56.05 }},
        {Ray{ Point3{ 0, -11.2, -0.1 }, Point3{ -7.21, -0.5, 0.9} },
            0.515, Point3{ -3.71315, -11.4575, 0.3635 }}
    }))};

    const Point3 res{ ray.at(param) };
    REQUIRE(res == exp);
}
