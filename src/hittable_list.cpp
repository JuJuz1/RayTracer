#include "hittable_list.h"

#include <utility>
#include <optional>

#include "ray.h"
#include "interval.h"
#include "hittable.h"

HittableList::HittableList() noexcept {};

HittableList::HittableList(std::unique_ptr<Hittable> object) { add(std::move(object)); }

void HittableList::add(std::unique_ptr<Hittable> object) { objects.push_back(std::move(object)); }

std::optional<HitRecord> HittableList::process_ray(const Ray& r, const Interval& ray_t) const noexcept {
    std::optional<HitRecord> closest_hit;
    double closest_so_far{ ray_t.max };

    for (const auto& obj : objects) {
        if (auto hit_rec{ obj->hit(r, Interval{ ray_t.min, closest_so_far }) }) {
            closest_hit = *hit_rec;
            closest_so_far = hit_rec->t;
        }
    }

    // closest_hit is {} if no object was hit
    return closest_hit;
}

void HittableList::clear() { objects.clear(); }

int HittableList::count() const noexcept {
    return static_cast<int>(objects.size());
}
