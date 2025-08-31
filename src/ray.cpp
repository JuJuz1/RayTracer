#include "ray.h"

Ray::Ray() noexcept : orig(Point3()), dir(Vec3()) {}

Ray::Ray(const Point3& origin, const Vec3& direction) noexcept : orig(origin), dir(direction) {}

const Point3& Ray::origin() const noexcept { return orig; }
const Point3& Ray::direction() const noexcept { return dir; }

Point3 Ray::at(double t) const noexcept { return orig + dir * t; }

std::ostream& operator<<(std::ostream& out, const Ray& r) {
    out << "Ray -> orig: " << r.origin() << " dir: " << r.direction();
    return out;
}