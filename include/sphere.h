#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "interval.h"

class Sphere final : public Hittable {
    public:
        Sphere(const Point3& center, double radius) noexcept;

        bool hit(const Ray& r, const Interval& ray_t, Hit_record& rec) const override;

    private:
        Point3 center;
        double radius;
};

#endif