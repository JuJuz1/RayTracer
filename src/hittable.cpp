#include "hittable.h"

void Hit_record::set_face_normal(const Ray& r, const Vec3& outward_normal) {
    // Assume outward_normal is unit length
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}