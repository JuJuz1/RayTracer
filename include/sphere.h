#ifndef INCLUDE_SPHERE_H_
#define INCLUDE_SPHERE_H_

#include "hittable.h"
#include "ray.h"
#include "interval.h"

// Sphere-specific data
struct SphereData {
    Vec3 center;
    double radius;
    Material* mat;
};

bool hit_sphere(const SphereData& s, const Ray& r, const Interval& ray_t, HitRecord& out_rec) noexcept;

#endif // INCLUDE_SPHERE_H_
