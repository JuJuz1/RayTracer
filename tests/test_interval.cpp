
#include "interval.h"

#include <limits>

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

constexpr double infinity{ std::numeric_limits<double>::infinity() };

constexpr double eps{ 1e-6 };
using Catch::Matchers::WithinAbs;

/*
Interval::empty and universe use rt::infinity -> std::numeric_limits<double>::infinity()
This has to be known here to avoid mixing up different infinities
*/

TEST_CASE("Size returns correct size", "[interval]") {
    const auto [inter, exp]{ GENERATE(table<Interval, double>({
        { { 1.0, 5.0 }, 4.0 },
        { { -9.8, -5.15 }, 4.65 },
        { { 6.1, 8.31 }, 2.21 },
        { Interval::empty, -infinity },
        { Interval::universe, infinity }
    }))};

    const double res{ inter.size() };
    REQUIRE_THAT(res, WithinAbs(exp, eps));
}

TEST_CASE("Clamp clamps the value correctly", "[interval]") {
    const auto [inter, param, exp]{ GENERATE(table<Interval, double, double>({
        { { 1.0, 5.0 }, 4.0, 4.0},
        { { -2.1, 9.2 }, -15, -2.1 },
        { { -15.1, 23.1 }, 59.123, 23.1 },
        { { -0.5, -6.1 }, -3.1, -0.5 },
        { Interval::empty, -12.1, infinity },
        { Interval::universe, 5.1, 5.1 }
    }))};

    const double res{ inter.clamp(param) };
    REQUIRE_THAT(res, WithinAbs(exp, eps));
}

TEST_CASE("Contains returns the correct result", "[interval]") {
    const auto [inter, param_x, param_incl, exp]{
        GENERATE(table<Interval, double, bool, bool>({
            { { 1.0, 5.0 }, 4.0, true, true},
            { { -1.2, -0.5 }, 0.2, false, false },
            { { 1, 5.86 }, 5.86, true, true },
            { { 1, 1.21 }, 0.99, false, false },
            { { 2.6, 2.9 }, 2.7, false, true },
            { Interval::empty, -12.1, false, false },
            { Interval::universe, 5.1, false, true }
    }))};

    const bool res{ inter.contains(param_x, param_incl) };
    REQUIRE(res == exp);
}
