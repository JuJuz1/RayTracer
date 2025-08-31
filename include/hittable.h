#pragma once

#include "ray.h"

// An abstract class for a hittable object

// Stores the information about the collision
struct Hit_record {
    Point3 p;
    Vec3 normal;
    double t;
};

class Hittable {
    public:
        virtual ~Hittable() = default;

        virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const = 0;
};