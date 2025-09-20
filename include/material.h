#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "color.h"

// An abstract material class to:
// 1. Produce a scattered ray
// 2. If scattered tell how much the ray should be attenuated (colored)
class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(
            const Ray& in_r, 
            const Hit_record& rec, 
            color& attenuation, 
            Ray& scattered) const noexcept;
};

// A material to scatter a ray always
class Lambertian : public Material {
    public:
        color albedo;
    
        Lambertian(const color& albedo);

        bool scatter(
            const Ray& in_r, 
            const Hit_record& rec, 
            color& attenuation, 
            Ray& scattered) const noexcept override;
};

#endif