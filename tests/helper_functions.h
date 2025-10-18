#ifndef TESTS_HELPER_FUNCTIONS_H_
#define TESTS_HELPER_FUNCTIONS_H_

#include "vec3.h"

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

inline void REQUIRE_VEC3_APPROX(const Vec3& act, const Vec3& exp, double eps = 1e-6) {
    using Catch::Matchers::WithinAbs;

    REQUIRE_THAT(act.x(), WithinAbs(exp.x(), eps));
    REQUIRE_THAT(act.y(), WithinAbs(exp.y(), eps));
    REQUIRE_THAT(act.z(), WithinAbs(exp.z(), eps));
}

#endif // TESTS_HELPER_FUNCTIONS_H_
