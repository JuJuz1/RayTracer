#include "hittable_list.h"

#include <utility>

HittableList::HittableList() noexcept {};

HittableList::HittableList(std::unique_ptr<Hittable> object) { add(std::move(object)); }

void HittableList::clear() { objects.clear(); }

void HittableList::add(std::unique_ptr<Hittable> object) { objects.push_back(std::move(object)); }

bool HittableList::hit(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept {
    HitRecord temp_rec;
    bool hit{ false };
    double closest_so_far{ ray_t.max };

    for (const auto& obj : objects) {
        if (obj->hit(r, Interval{ ray_t.min, closest_so_far }, temp_rec)) {
            hit = true;
            closest_so_far = temp_rec.t;
            out_rec = temp_rec;
        }
    }

    return hit;
}

int HittableList::count() const noexcept {
    return static_cast<int>(objects.size());
}
