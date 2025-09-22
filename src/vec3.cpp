#include "vec3.h"

#include <cmath>

#include "rtweekend.h"

Vec3::Vec3() noexcept : e{ 0, 0, 0 } {}

Vec3::Vec3(double e0, double e1, double e2) noexcept : e{ e0, e1, e2 } {}

constexpr double Vec3::x() const noexcept { return e[0]; }
constexpr double Vec3::y() const noexcept { return e[1]; }
constexpr double Vec3::z() const noexcept { return e[2]; }

Vec3 Vec3::operator-() const noexcept { return Vec3{ -e[0], -e[1], -e[2] }; }

constexpr double Vec3::operator[](int i) const noexcept { return e[i]; }
constexpr double& Vec3::operator[](int i) noexcept { return e[i]; }

Vec3& Vec3::operator+=(const Vec3& other) noexcept {
    e[0] += other.e[0];
    e[1] += other.e[1];
    e[2] += other.e[2];
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) noexcept {
    e[0] -= other.e[0];
    e[1] -= other.e[1];
    e[2] -= other.e[2];
    return *this;
}

Vec3& Vec3::operator*=(double t) noexcept {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

Vec3& Vec3::operator/=(double t) noexcept {
    return *this *= (1/t);
}

double Vec3::length() const noexcept {
    return std::sqrt(length_squared());
}

constexpr double Vec3::length_squared() const noexcept {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

bool Vec3::is_near_zero() const noexcept {
    constexpr double eps = 1e-8;
    return std::fabs(e[0]) < eps
        && std::fabs(e[1]) < eps
        && std::fabs(e[2]) < eps;
}

// Utility functions

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    out << v.x() << ", " << v.y() << ", " << v.z();
    return out;
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3{ v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z() };
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3{ v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z() };
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3{ v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z() };
}

Vec3 operator*(const Vec3& v, double t) noexcept {
    return Vec3{ v.x() * t, v.y() * t, v.z() * t };
}

// Calls operator*(const Vec3& v, double t)
Vec3 operator/(const Vec3& v, double t) noexcept {
    return v * (1/t);
}

constexpr double dot(const Vec3& v1, const Vec3& v2) noexcept {
    return v1.x() * v2.x()
         + v1.y() * v2.y()
         + v1.z() * v2.z();
}

Vec3 cross(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3{v1.y() * v2.z() - v1.z() * v2.y(),
                v1.z() * v2.x() - v1.x() * v2.z(),
                v1.x() * v2.y() - v1.y() * v2.x()};
}

Vec3 unit_vector(const Vec3& v) noexcept {
    return v / v.length();
}

Vec3 random_vector() noexcept {
    return Vec3{ rt::random_double(), rt::random_double(), rt::random_double() };
}

Vec3 random_vector(double min, double max) noexcept {
    return Vec3{rt::random_double(min, max), 
                rt::random_double(min, max), 
                rt::random_double(min, max)};
}

Vec3 random_unit_vector() noexcept {
    Vec3 p;
    double lensq;
    while (true) {
        p = random_vector(-1, 1);
        lensq = p.length_squared();
        // Prevent lensq underflow to zero
        if (1e-160 < lensq && lensq <= 1)
            return p / std::sqrt(lensq);
    }
}

Vec3 random_in_unit_disk() noexcept {
    Vec3 p;
    while (true) {
        p = Vec3{ rt::random_double(-1, 1), rt::random_double(-1.0, 1.0), 0 };
        if (p.length_squared() < 1)
            return p;
    }
}

Vec3 random_on_hemisphere(const Vec3& normal) noexcept {
    const Vec3 on_unit_sphere{ random_unit_vector() };
    // Same hemisphere as normal
    if (dot(on_unit_sphere, normal) > 0)
        return on_unit_sphere;
    
    return -on_unit_sphere;
}

Vec3 reflect(const Vec3& v, const Vec3& n) noexcept {
    return v - n * dot(v, n) * 2;
}

Vec3 refract(const Vec3& v, const Vec3& n, double etai_over_etat) noexcept {
    const double cos_theta{ std::fmin(dot(-v, n), 1.0) };
    Vec3 r_out_perp{ (n * cos_theta + v) * etai_over_etat };
    Vec3 r_out_parallel{ n * -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) };
    return r_out_perp + r_out_parallel;
}