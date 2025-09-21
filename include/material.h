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

        // TODO: check this works correctly
        // Every hittable object needs a material so we will
        // use this base class as the material and just return false
        virtual bool scatter(
            const Ray& in_r, 
            const Hit_record& rec, 
            color& attenuation, 
            Ray& scattered) const noexcept;
};

// A material to always scatter a ray
class Lambertian : public Material {
    public:
        Lambertian(const color& albedo);

        bool scatter(
            const Ray& in_r, 
            const Hit_record& rec, 
            color& out_attenuation, 
            Ray& out_scattered) const noexcept override;
    
    private:
        color albedo;
};

// Reflects a ray
class Metal : public Material {
    public:
        Metal(const color& albedo, double fuzz);

        // Fuzzy reflection
        bool scatter(
            const Ray& in_r, 
            const Hit_record& rec, 
            color& out_attenuation, 
            Ray& out_scattered) const noexcept override;
        
    private:
        color albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double refraction_index);

        // Refraction
        bool scatter(
            const Ray& in_r, 
            const Hit_record& rec, 
            color& out_attenuation, 
            Ray& out_scattered) const noexcept override;
        
    private:
        double refraction_index;
};

#endif