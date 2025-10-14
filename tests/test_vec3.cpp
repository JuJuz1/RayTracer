#include "vec3.h"

#include "helper_functions.h"

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

constexpr double eps{ 1e-4 };
using Catch::Matchers::WithinAbs;

/*
Decided not to use Vec3::operator== for comparisons
That way it's harder to define the value for epsilon
Also epsilon is a little more forgiving here (1e-4) so that
scalar division tests pass without needing to specify too many digits
*/

TEST_CASE("Length returns correct length", "[vec3]") {
    const auto [vec, exp]{ GENERATE(table<Vec3, double>({
        { {1.0, 2.0, 3.5}, 4.1533 },
        { {-0.1, -1.23, 0.1}, 1.2381 },
        { {-5.5, 0.0, 8.1}, 9.7908 }
    }))};

    const double len{ vec.length() };
    REQUIRE_THAT(len, WithinAbs(exp, eps));
}

TEST_CASE("Length squared returns correct length", "[vec3]") {
    const auto [vec, exp]{ GENERATE(table<Vec3, double>({
        { { 1.0, 2.0, 3.5 }, 17.25 },
        { { -0.1, -1.23, 0.1 }, 1.5329 },
        { { -5.5, 0.0, 8.1 }, 95.86 }
    }))};

    const double len_sqr{ vec.length_squared() };
    REQUIRE_THAT(len_sqr, WithinAbs(exp, eps));
}

// Utility functions

TEST_CASE("Vec3 addition works correctly", "[vec3]") {
    const auto [a, b, exp]{ GENERATE(table<Vec3, Vec3, Vec3>({
        { { 1.0, 2.0, 11.51 }, { 2.0, 0.87, -2.1 }, { 3.0, 2.87, 9.41 } },
        { { -0.0, -1.231, 0.1 }, { 0.0, 0.912, -1.2 }, { 0.0, -0.319, -1.1 } },
        { { -0.9, -2.5, 3.0 }, { 0.0, 0.0, 0.0 }, { -0.9, -2.5, 3.0 } }
    }))};

    const Vec3 res{ a + b };
    REQUIRE_VEC3_APPROX(res, exp, eps);
}

TEST_CASE("Vec3 subtraction works correctly", "[vec3]") {
    const auto [a, b, exp]{ GENERATE(table<Vec3, Vec3, Vec3>({
        { { 1, 21.8, 3.1 }, { 2.2, 5.12, 8 }, { -1.2, 16.68, -4.9 } },
        { { -0.12, -0.2, 3.1 }, {-2.1, 0.5, -9 }, { 1.98, -0.7, 12.1 } },
        { { -0.1, -0.2, 3.0 }, { -0.1, -0.2, 3.0 }, { 0.0, 0.0, 0.0 } }
    }))};

    const Vec3 res{ a - b };
    REQUIRE_VEC3_APPROX(res, exp, eps);
}

TEST_CASE("Vec3 component-wise multiplication works correctly", "[vec3]") {
    const auto [a, b, exp] = GENERATE(table<Vec3, Vec3, Vec3>({
        { { 1.0, 2.31, 3.0 }, { 2.0, 0.51, -1.97 }, { 2.0, 1.1781, -5.91 } },
        { { -1.5, 0.0, 4.25 }, { 2.31, -3.2, 0.5 }, { -3.465, -0.0, 2.125 } },
        { { 0.1, -2.2, 5.0 }, { 10.0, 0.512, 0.123 }, { 1.0, -1.1264, 0.615 } }
    }));

    const Vec3 res{ a * b };
    REQUIRE_VEC3_APPROX(res, exp, eps);
}

TEST_CASE("Vec3 scalar multiplication works correctly", "[vec3]") {
    const auto [vec, t, exp] = GENERATE(table<Vec3, double, Vec3>({
        { { 3.14, -1.25, 0.0 }, 1.5, { 4.71, -1.875, 0.0 } },
        { { -2.7, 0.33, 7.1 }, -0.8, { 2.16, -0.264, -5.68 } },
        { { 0.01, 2.31, -4.2 }, 3.2, { 0.032, 7.392, -13.44 } }
    }));

    const Vec3 res{ vec * t };
    REQUIRE_VEC3_APPROX(res, exp, eps);
}

TEST_CASE("Vec3 scalar division works correctly", "[vec3]") {
    const auto [vec, t, exp] = GENERATE(table<Vec3, double, Vec3>({
        { { 1.0, 2.31, 3.0 }, 2.31, { 0.4329, 1.0, 1.2987 } },
        { { -1.5, 0.0, 4.25 }, -2.0, { 0.75, 0.0, -2.125 } },
        { { 0.1, -2.2, 5.0 }, 0.51, { 0.1961, -4.3137, 9.8039 } }
    }));

    const Vec3 res{ vec / t };
    REQUIRE_VEC3_APPROX(res, exp, eps);
}

TEST_CASE("Dot returns correct result", "[vec3]") {
    const auto [a, b, exp] = GENERATE(table<Vec3, Vec3, double>({
        { { 1.0, 2.31, 3.0 }, { 0.12, 1.0, 1.2987 }, 6.3261 },
        { { 3.14, -1.25, 0.5 }, { 1.5, -2.0, 4.2 }, 9.31 },
        { { -0.33, 2.71, -1.1 }, { 0.25, -0.5, 3.14 }, -4.8915 }
    }));

    const double res{ dot(a, b) };
    REQUIRE_THAT(res, WithinAbs(exp, eps));
}

TEST_CASE("Cross returns correct result", "[vec3]") {
    const auto [a, b, exp] = GENERATE(table<Vec3, Vec3, Vec3>({
        { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { -3.0, 6.0, -3.0 } },
        { { -1.5, 0.0, 4.25 }, { 0.75, -2.0, -2.0 }, { 8.5, 0.1875, 3.0 } },
        { { 0.1, -2.2, 5.0 }, { 0.5, 1.0, -0.2 }, { -4.56, 2.52, 1.2 } }
    }));

    const Vec3 res{ cross(a, b) };
    REQUIRE_VEC3_APPROX(res, exp, eps);
}

/*
TODO:
- unit_vector
- reflect
- refract
*/