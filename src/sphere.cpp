#include "sphere.h"

#include <memory>
#include <cmath>
#include <optional>

#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "hittable.h"

Sphere::Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat) noexcept
    : center{ center }, radius{ std::fmax(radius, 0.0) }, mat{ mat } {}

std::optional<HitRecord> Sphere::hit(const Ray& r, const Interval& ray_t) const noexcept {
    Vec3 oc{ center - r.origin() };
    const double a{ r.direction().length_squared() };
    const double h{ dot(r.direction(), oc) };
    const double c{ oc.length_squared() - radius * radius };
    const double discriminant{ h * h - a * c };

    // No roots
    if (discriminant < 0.0)
        return {};

    const double sqrt_d{ std::sqrt(discriminant) };
    // Find the nearest root in the acceptable range: ray_t.min < t < ray_t.max
    double root{ (h - sqrt_d) / a };
    if (!ray_t.contains(root, false)) {
        root = (h + sqrt_d) / a;
        if (!ray_t.contains(root, false))
            return {};
    }

    HitRecord rec;
    rec.t = root;
    rec.p = r.at(rec.t);
    const Vec3 outward_normal{ (rec.p - center) / radius };
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return rec;
}
