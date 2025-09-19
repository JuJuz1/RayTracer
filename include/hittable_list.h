#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>

#include "hittable.h"

// A class for all hittable objects
class Hittable_list : public Hittable {
    public:
        std::vector<std::unique_ptr<Hittable>> objects;

        Hittable_list() noexcept;
        Hittable_list(std::unique_ptr<Hittable>);

        void clear();
        void add(std::unique_ptr<Hittable> object);

        bool hit(const Ray& r, const Interval& ray_t, Hit_record& out_rec) const noexcept override;
};

#endif