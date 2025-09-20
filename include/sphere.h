#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"

class Sphere final : public Hittable {
    public:
        Sphere(const Point3& center, double radius) noexcept;

        bool hit(const Ray& r, const Interval& ray_t, Hit_record& out_rec) const noexcept override;

    private:
        Point3 center;
        double radius;
        // Sphere also needs to know the material
        std::shared_ptr<Material> mat;
};

#endif