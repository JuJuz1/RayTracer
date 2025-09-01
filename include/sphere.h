#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere final : public Hittable {
    public:
        Sphere(const Point3& center, double radius) noexcept;

        bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const override;

    private:
        Point3 center;
        double radius;
};

#endif