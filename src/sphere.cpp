#include "sphere.h"
#include "interval.h"

Sphere::Sphere(const Point3& center, double radius) noexcept 
    : center(center), radius(std::fmax(radius, 0.0)) {}

bool Sphere::hit(const Ray& r, const Interval& ray_t, Hit_record& out_rec) const noexcept {
    Vec3 oc{ center - r.origin() };
    const double a = r.direction().length_squared();
    const double h = dot(r.direction(), oc);
    const double c = oc.length_squared() - radius * radius;
    const double discriminant = h * h - 4 * a * c;

    // No roots
    if (discriminant < 0)
        return false;

    const double sqrt_d = std::sqrt(discriminant);
    // Find the nearest root in the acceptable range: ray_t.min < t < ray_t.max
    double root = (h - sqrt_d) / a;
    if (!ray_t.contains(root, false)) {
        root = (h + sqrt_d) / a;
        if (!ray_t.contains(root, false))
            return false;
    }

    out_rec.t = root;
    out_rec.p = r.at(out_rec.t);
    const Vec3 outward_normal{ (out_rec.p - center) / radius };
    out_rec.set_face_normal(r, outward_normal);

    return true;
}