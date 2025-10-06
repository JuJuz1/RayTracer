#include "hittable_list.h"

#include <utility>

#include "vec3.h"
#include "material.h"
#include "sphere.h"
#include "hittable.h"
#include "ray.h"
#include "interval.h"

HittableList::HittableList() noexcept {};

void HittableList::add_sphere(const Vec3& center, double radius, Material& mat) noexcept {
    SphereData* s = new SphereData{ center, radius, &mat };
    objects.push_back(Hittable{ HittableType::Sphere, s });
}

bool HittableList::process_ray(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept {
    HitRecord temp_rec;
    bool hit{ false };
    double closest_so_far{ ray_t.max };

    for (const auto& obj : objects) {
        bool hit_current{ false };

        switch (obj.type) {
            case HittableType::Sphere: {
                SphereData* s = static_cast<SphereData*>(obj.data);
                hit_current = hit_sphere(*s, r, Interval{ ray_t.min, closest_so_far }, temp_rec);
            } break;

            default: break;
        }

        if (hit_current) {
            hit = true;
            closest_so_far = temp_rec.t;
            out_rec = temp_rec;
        }
    }

    return hit;
}

void HittableList::clear() { objects.clear(); }

int HittableList::count() const noexcept {
    return static_cast<int>(objects.size());
}
