#include "sphere.h"

Sphere::Sphere(const Point3& center, double radius) noexcept 
    : center(center), radius(std::fmax(radius, 0.0)) {}

bool Sphere::hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const {
    Vec3 oc{center - r.origin()};
    const double a = r.direction().length_squared();
    const double h = dot(r.direction(), oc);
    const double c = oc.length_squared() - radius * radius;
    const double discriminant = h * h - 4 * a * c;

    // No roots
    if (discriminant < 0)
        return false;

    const double sqrt_d = std::sqrt(discriminant);
    // Find the nearest root in the acceptable range: ray_tmin < t < ray_tmax
    double root = (h - sqrt_d) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrt_d) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    const Vec3 outward_normal{(rec.p - center) / radius};
    rec.set_face_normal(r, outward_normal);

    return true;
}