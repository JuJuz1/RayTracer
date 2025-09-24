#ifndef INCLUDE_HITTABLE_H_
#define INCLUDE_HITTABLE_H_

#include <memory>

#include "vec3.h"
#include "ray.h"
#include "interval.h"

// TODO: maybe just include material.h
// Forward declare Material to allow HitRecord
// to contain shared ptr of Material
class Material;

// Stores the information about the collision
struct HitRecord {
    Point3 p;
    Vec3 normal;
    // Aby object which inherits hittable
    // needs to have a material reference, hence the shared ptr
    std::shared_ptr<Material> mat;
    double t;
    bool front_face;

    // Updates normal to always point outwards from the surface
    // Also modifies front_face to tell if the hit is inside the hittable object
    void set_face_normal(const Ray& r, const Vec3& outward_normal) noexcept;
};

// An abstract class for a hittable object
class Hittable {
 public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, const Interval& ray_t, HitRecord& rec) const noexcept = 0;
};

#endif // INCLUDE_HITTABLE_H_
