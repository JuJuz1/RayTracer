#ifndef VEC3_H
#define VEC3_H

#include <iostream>

// As the book stated, it might be more efficient to use float instead of double
// for now we are going to use double as we are not working in limited memory conditions

class Vec3 {
    public:
        Vec3() noexcept;

        Vec3(double e0, double e1, double e2) noexcept;

        constexpr double x() const noexcept;
        constexpr double y() const noexcept;
        constexpr double z() const noexcept;

        // Operators
        Vec3 operator-() const noexcept;

        constexpr double operator[](int i) const noexcept;
        constexpr double& operator[](int i) noexcept;

        Vec3& operator+=(const Vec3& other) noexcept;
        Vec3& operator-=(const Vec3& other) noexcept;
        Vec3& operator*=(double t) noexcept;
        Vec3& operator/=(double t) noexcept;

        double length() const noexcept;
        constexpr double length_squared() const noexcept;

        // Checks if a vector is close to zero in all dimensions
        bool is_near_zero() const noexcept;
    
    private:
        double e[3];
};

// An alias for geometric clarity (e.g. ray.h)
using Point3 = Vec3;

// Utility functions

std::ostream& operator<<(std::ostream& out, const Vec3& v);

Vec3 operator+(const Vec3& v1, const Vec3& v2) noexcept;
Vec3 operator-(const Vec3& v1, const Vec3& v2) noexcept;
Vec3 operator*(const Vec3& v1, const Vec3& v2) noexcept;

Vec3 operator*(const Vec3& v, double t) noexcept;
Vec3 operator/(const Vec3& v, double t) noexcept;

constexpr double dot(const Vec3& v1, const Vec3& v2) noexcept;
Vec3 cross(const Vec3& v1, const Vec3& v2) noexcept;

Vec3 unit_vector(const Vec3& v) noexcept;

Vec3 random_vector() noexcept;
Vec3 random_vector(double min, double max) noexcept;
// Returns a random unit vector that is inside a unit sphere
Vec3 random_unit_vector() noexcept;

// Finds a random vector from a point where the ray hit
// and checks whether the vector is in the correct hemisphere
Vec3 random_on_hemisphere(const Vec3& normal) noexcept;

// Returns the reflection of v about the surface normal n
Vec3 reflect(const Vec3& v, const Vec3& n) noexcept;

#endif