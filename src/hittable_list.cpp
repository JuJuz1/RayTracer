#include "hittable_list.h"
#include "interval.h"

Hittable_list::Hittable_list() noexcept {};

Hittable_list::Hittable_list(std::unique_ptr<Hittable> object) noexcept { add(std::move(object)); }

void Hittable_list::clear() { objects.clear(); }
void Hittable_list::add(std::unique_ptr<Hittable> object) { objects.push_back(std::move(object)); }

bool Hittable_list::hit(const Ray& r, const Interval& ray_t, Hit_record& rec) const {
    Hit_record temp_rec;
    bool hit = false;
    double closest_so_far = ray_t.max;

    for (const auto& obj : objects) {
        if (obj->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
            hit = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit;
}