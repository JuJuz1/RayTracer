#ifndef INCLUDE_VEC3_H_
#define INCLUDE_VEC3_H_

#include <ostream>

// As the book stated, it might be more efficient to use float instead of double
// for now we are going to use double as we are not working in limited memory conditions

class Vec3 {
 public:
    // Need to define here for color.h
    constexpr Vec3() noexcept : e{ 0, 0, 0 } {}
    constexpr Vec3(double e0, double e1, double e2) noexcept : e{ e0, e1, e2 } {}

    [[nodiscard]] double x() const noexcept;
    [[nodiscard]] double y() const noexcept;
    [[nodiscard]] double z() const noexcept;

    [[nodiscard]] double operator[](int i) const noexcept;
    [[nodiscard]] double& operator[](int i) noexcept;

    [[nodiscard]] Vec3 operator-() const noexcept;

    Vec3& operator+=(const Vec3& other) noexcept;
    Vec3& operator-=(const Vec3& other) noexcept;
    Vec3& operator*=(double t) noexcept;
    Vec3& operator/=(double t) noexcept;

    [[nodiscard]] double length() const noexcept;
    [[nodiscard]] double length_squared() const noexcept;

    // Checks if a vector is close to zero in all dimensions
    bool is_near_zero() const noexcept;

 private:
    double e[3];
};

// An alias for geometric clarity (e.g. ray.h)
using Point3 = Vec3;

// Utility functions

std::ostream& operator<<(std::ostream& out, const Vec3& v);

[[nodiscard]] bool operator==(const Vec3& v1, const Vec3& v2) noexcept;
[[nodiscard]] bool operator!=(const Vec3& v1, const Vec3& v2) noexcept;

[[nodiscard]] Vec3 operator+(const Vec3& v1, const Vec3& v2) noexcept;
[[nodiscard]] Vec3 operator-(const Vec3& v1, const Vec3& v2) noexcept;

[[nodiscard]] Vec3 operator*(const Vec3& v1, const Vec3& v2) noexcept;
[[nodiscard]] Vec3 operator*(const Vec3& v, double t) noexcept;
[[nodiscard]] Vec3 operator/(const Vec3& v, double t) noexcept;

[[nodiscard]] double dot(const Vec3& v1, const Vec3& v2) noexcept;
[[nodiscard]] Vec3 cross(const Vec3& v1, const Vec3& v2) noexcept;

// Returns the unit represenation of v
[[nodiscard]] Vec3 unit_vector(const Vec3& v) noexcept;

// Returns a random vector with all elements in the range [0.0, 1.0[
[[nodiscard]] Vec3 random_vector() noexcept;

// Returns a random vector with all elements in the range [min, max[
[[nodiscard]] Vec3 random_vector(double min, double max) noexcept;

// Returns a random unit vector that is inside a unit sphere
[[nodiscard]] Vec3 random_unit_vector() noexcept;

// Retrurns a random (2D) vector that is inside a unit disk
[[nodiscard]] Vec3 random_in_unit_disk() noexcept;

// Finds a random vector from a point where the ray hit
// and checks whether the vector is in the correct hemisphere
[[nodiscard]] Vec3 random_on_hemisphere(const Vec3& normal) noexcept;

// Returns the reflection of v about the surface normal n
[[nodiscard]] Vec3 reflect(const Vec3& v, const Vec3& n) noexcept;

// Returns a refracted vector of v passing through a surface with normal n
// etai_over_etat is the ratio of indices of refraction
// -> refractive index of the material / the material v is entering
// Implemented with Snell's law
[[nodiscard]] Vec3 refract(const Vec3& v, const Vec3& n, double etai_over_etat) noexcept;

#endif // INCLUDE_VEC3_H_
