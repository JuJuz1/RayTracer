#include "hittable_list.h"

Hittable_list::Hittable_list() noexcept {};

Hittable_list::Hittable_list(std::unique_ptr<Hittable> object) noexcept { add(std::move(object)); }

void Hittable_list::clear() { objects.clear(); }
void Hittable_list::add(std::unique_ptr<Hittable> object) { objects.push_back(std::move(object)); }

bool Hittable_list::hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const {
    Hit_record temp_rec;
    bool hit = false;
    double closest = ray_tmax;

    for (const auto& obj : objects) {
        if (obj->hit(r, ray_tmin, closest, temp_rec)) {
            hit = true;
            closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit;
}