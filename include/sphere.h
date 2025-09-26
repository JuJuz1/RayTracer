#ifndef INCLUDE_SPHERE_H_
#define INCLUDE_SPHERE_H_

#include <memory>

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "material.h"

class Sphere final : public Hittable {
 public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat) noexcept;

    bool hit(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept override;

 private:
    Point3 center;
    double radius;
    // Sphere also needs the material
    std::shared_ptr<Material> mat;
};

#endif // INCLUDE_SPHERE_H_
