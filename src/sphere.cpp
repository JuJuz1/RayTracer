#include "sphere.h"

#include <cmath>

#include "hittable.h"
#include "ray.h"
#include "interval.h"

bool hit_sphere(const SphereData& s, const Ray& r, const Interval& ray_t, HitRecord& out_rec) noexcept {
    Vec3 oc{ s.center - r.origin() };
    const double a{ r.direction().length_squared() };
    const double h{ dot(r.direction(), oc) };
    const double c{ oc.length_squared() - s.radius * s.radius };
    const double discriminant{ h * h - a * c };

    // No roots
    if (discriminant < 0)
        return false;

    const double sqrt_d{ std::sqrt(discriminant) };
    // Find the nearest root in the acceptable range: ray_t.min < t < ray_t.max
    double root{ (h - sqrt_d) / a };
    if (!ray_t.contains(root, false)) {
        root = (h + sqrt_d) / a;
        if (!ray_t.contains(root, false))
            return false;
    }

    out_rec.t = root;
    out_rec.p = r.at(out_rec.t);
    const Vec3 outward_normal{ (out_rec.p - s.center) / s.radius };
    out_rec.set_face_normal(r, outward_normal);
    out_rec.mat = s.mat;

    return true;
}
