#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray() noexcept;
        Ray(const Point3& origin, const Vec3& direction) noexcept;

        const Point3& origin() const noexcept;
        const Vec3& direction() const noexcept;

        // Returns the point along the ray when moving t in the direction
        Point3 at(double t) const noexcept;

    private:
        Point3 orig;
        Vec3 dir;
};

std::ostream& operator<<(std::ostream& out, const Ray& r);

#endif