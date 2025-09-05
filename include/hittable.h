#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

// Stores the information about the collision
struct Hit_record {
    Point3 p;
    Vec3 normal;
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

        virtual bool hit(const Ray& r, const Interval& ray_t, Hit_record& rec) const noexcept = 0;
};

#endif