#ifndef INCLUDE_HITTABLE_LIST_H_
#define INCLUDE_HITTABLE_LIST_H_

#include <vector>
#include <memory>

#include "hittable.h"
#include "ray.h"
#include "interval.h"

// A class for all hittable objects
class HittableList : public Hittable {
 public:
    HittableList() noexcept;
    explicit HittableList(std::unique_ptr<Hittable>);

    void clear();
    void add(std::unique_ptr<Hittable> object);

    bool hit(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept override;

    // Get the count of hittable objects
    int count() const noexcept;

 private:
    std::vector<std::unique_ptr<Hittable>> objects;
};

#endif // INCLUDE_HITTABLE_LIST_H_
