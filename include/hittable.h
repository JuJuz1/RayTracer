#ifndef INCLUDE_HITTABLE_H_
#define INCLUDE_HITTABLE_H_

#include "vec3.h"
#include "ray.h"
#include "material.h"
#include "interval.h"

// Needed for SphereData
struct Material;

// Stores the information about the collision
struct HitRecord {
    Point3 p;
    Vec3 normal;
    Material* mat;
    double t;
    bool front_face;

    // Updates normal to always point outwards from the surface
    // Also modifies front_face to tell if the hit is inside the hittable object
    void set_face_normal(const Ray& r, const Vec3& outward_normal) noexcept;
};

enum class HittableType {
    Sphere,

    MaxCount
};

struct Hittable {
    HittableType type;
    void* data;
};

#endif // INCLUDE_HITTABLE_H_
