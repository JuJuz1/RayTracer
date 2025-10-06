#include "hittable_list.h"

#include <utility>

#include "ray.h"
#include "interval.h"
#include "hittable.h"

HittableList::HittableList() noexcept {};

void HittableList::add_sphere(const Vec3& center, double radius, std::shared_ptr<Material> mat) noexcept {
    SphereData* s = new SphereData{ center, radius, mat };
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

        // if (obj->hit(r, Interval{ ray_t.min, closest_so_far }, temp_rec)) {
        //     hit = true;
        //     closest_so_far = temp_rec.t;
        //     out_rec = temp_rec;
        // }

    }

    return hit;
}

void HittableList::clear() { objects.clear(); }

int HittableList::count() const noexcept {
    return static_cast<int>(objects.size());
}

bool hit_sphere(const SphereData& s, const Ray& r, const Interval& ray_t, HitRecord& out_rec) noexcept {
    Vec3 oc{ s.center - r.origin() };
    const double a{ r.direction().length_squared() };
    const double h{ dot(r.direction(), oc) };
    const double c{ oc.length_squared() - s.radius * s.radius };
    const double discriminant{ h * h - a * c };

    // No roots
    if (discriminant < 0)
        return false;

    const double sqrt_d{ std::sqrt(discriminant) };
    // Find the nearest root in the acceptable range: ray_t.min < t < ray_t.max
    double root{ (h - sqrt_d) / a };
    if (!ray_t.contains(root, false)) {
        root = (h + sqrt_d) / a;
        if (!ray_t.contains(root, false))
            return false;
    }

    out_rec.t = root;
    out_rec.p = r.at(out_rec.t);
    const Vec3 outward_normal{ (out_rec.p - s.center) / s.radius };
    out_rec.set_face_normal(r, outward_normal);
    out_rec.mat = s.mat;

    return true;
}
