#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

// A class for all hittable objects
class Hittable_list : public Hittable {
    public:
        std::vector<std::shared_ptr<Hittable>> objects;

        Hittable_list() noexcept;
        Hittable_list(std::shared_ptr<Hittable>) noexcept;

        void clear();
        void add(std::shared_ptr<Hittable> object);

        bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const override;
};