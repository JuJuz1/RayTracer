#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>

#include "hittable.h"

// A class for all hittable objects
class HittableList : public Hittable {
    public:
        std::vector<std::unique_ptr<Hittable>> objects;

        HittableList() noexcept;
        HittableList(std::unique_ptr<Hittable>);

        void clear();
        void add(std::unique_ptr<Hittable> object);

        bool hit(const Ray& r, const Interval& ray_t, HitRecord& out_rec) const noexcept override;

        // Get the count of hittable objects
        int count() const noexcept;
};

#endif