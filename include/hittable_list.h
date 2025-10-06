#ifndef INCLUDE_HITTABLE_LIST_H_
#define INCLUDE_HITTABLE_LIST_H_

#include <vector>
#include <memory>

#include "ray.h"
#include "interval.h"
#include "hittable.h"

// A class for all hittable objects
class HittableList {
 public:
    HittableList() noexcept;

    void HittableList::add_sphere(const Vec3& center, double radius, std::shared_ptr<Material> mat) noexcept;

    // Processess through all the objects and calls hit on every Hittable that was hit
    bool process_ray(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept;

    // Clear the list
    void clear();

    // Get the count of hittable objects
    int count() const noexcept;

 private:
    std::vector<Hittable> objects;

};

bool hit_sphere(const SphereData& s, const Ray& r, const Interval& ray_t, HitRecord& out_rec) noexcept;

#endif // INCLUDE_HITTABLE_LIST_H_
