#pragma once

#include <vec3.h>

class Ray {
    public:
        Ray();
        Ray(const Point3& origin, const Vec3& direction);

        const Point3& origin() const;
        const Vec3& direction() const;

        // TODO: better comment
        // Returns the point along the ray when moving the amount t in the direction
        Point3 at(double t);

    private:
        Point3 orig;
        Vec3 dir;
};

std::ostream& operator<<(std::ostream& out, const Ray& r);