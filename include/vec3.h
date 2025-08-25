#pragma once

#include <cmath>
#include <iostream>

// As the book stated, it might be more efficient to use float instead of double
// for now we are going to use double as we are not working in limited memory conditions

class Vec3 {
    public:
        double e[3];

        Vec3() noexcept;
        Vec3(double e0, double e1, double e2) noexcept;

        double x() const noexcept;
        double y() const noexcept;
        double z() const noexcept;

        // Operators
        Vec3 operator-() const noexcept;

        double operator[](int i) const noexcept;
        double& operator[](int i) noexcept;

        Vec3& operator+=(const Vec3& other) noexcept;
        Vec3& operator-=(const Vec3& other) noexcept;
        Vec3& operator*=(double t) noexcept;
        Vec3& operator/=(double t) noexcept;

        double length() const noexcept;
        double length_squared() const noexcept;
};

// An alias for geometric clarity (t. ex. ray.h)
using Point3 = Vec3;

// Utility functions
std::ostream& operator<<(std::ostream& out, const Vec3& v);

Vec3 operator+(const Vec3& v1, const Vec3& v2) noexcept;
Vec3 operator-(const Vec3& v1, const Vec3& v2) noexcept;
Vec3 operator*(const Vec3& v1, const Vec3& v2) noexcept;

Vec3 operator*(const Vec3& v, double t) noexcept;
Vec3 operator/(const Vec3& v, double t) noexcept;

double dot(const Vec3& v1, const Vec3& v2) noexcept;
Vec3 cross(const Vec3& v1, const Vec3& v2) noexcept;

Vec3 unit_vector(const Vec3& v) noexcept;