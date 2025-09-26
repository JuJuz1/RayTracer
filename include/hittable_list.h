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
    explicit HittableList(std::unique_ptr<Hittable>);

    void clear();
    void add(std::unique_ptr<Hittable> object);

    // Processess through all the objects and calls hit on every Hittable that was hit
    bool process_ray(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept;

    // Get the count of hittable objects
    int count() const noexcept;

 private:
    std::vector<std::unique_ptr<Hittable>> objects;
};

#endif // INCLUDE_HITTABLE_LIST_H_
