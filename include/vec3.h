#pragma once

#include <cmath>
#include <iostream>

// As the book stated, it might be more efficient to use float instead of double
// for now we are going to use double as we are not working in limited memory conditions

class Vec3 {
    public:
        double e[3];

        Vec3();
        Vec3(double e0, double e1, double e2);

        double x() const;
        double y() const;
        double z() const;

        // Operators
        Vec3 operator-() const;

        double operator[](int i) const;
        double& operator[](int i);

        Vec3& operator+=(const Vec3& other);
        Vec3& operator-=(const Vec3& other);
        Vec3& operator*=(double t);
        Vec3& operator/=(double t);

        double length() const;
        double length_squared() const;
};

// An alias for geometric clarity (t. ex. ray.h)
using Point3 = Vec3;

// Utility functions
std::ostream& operator<<(std::ostream& out, const Vec3& v);

Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v1, const Vec3& v2);

Vec3 operator*(const Vec3& v, double t);
Vec3 operator/(const Vec3& v, double t);

double dot(const Vec3& v1, const Vec3& v2);
Vec3 cross(const Vec3& v1, const Vec3& v2);

Vec3 unit_vector(const Vec3& v);