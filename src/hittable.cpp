#include "hittable.h"

void HitRecord::set_face_normal(const Ray& r, const Vec3& outward_normal) noexcept {
    // Assume outward_normal is unit length
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}
