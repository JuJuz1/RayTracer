#include "vec3.h"

Vec3::Vec3() noexcept : e{0, 0, 0} {}

Vec3::Vec3(double e0, double e1, double e2) noexcept : e{e0, e1, e2} {}

double Vec3::x() const noexcept { return e[0]; }
double Vec3::y() const noexcept { return e[1]; }
double Vec3::z() const noexcept { return e[2]; }

Vec3 Vec3::operator-() const noexcept { return Vec3( -e[0], -e[1], -e[2]); }

double Vec3::operator[](int i) const noexcept { return e[i]; }
double& Vec3::operator[](int i) noexcept { return e[i]; }

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

double Vec3::length_squared() const noexcept {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

// Utility functions

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    out << v.e[0] << ", " << v.e[1] << ", " << v.e[2];
    return out;
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

Vec3 operator*(const Vec3& v, double t) noexcept {
    return Vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

Vec3 operator/(const Vec3& v, double t) noexcept {
    return v * (1/t);
}

double dot(const Vec3& v1, const Vec3& v2) noexcept {
    return v1.e[0] * v2.e[0]
         + v1.e[1] * v2.e[1]
         + v1.e[2] * v2.e[2];
}

Vec3 cross(const Vec3& v1, const Vec3& v2) noexcept {
    return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

Vec3 unit_vector(const Vec3& v) noexcept {
    return v / v.length();
}