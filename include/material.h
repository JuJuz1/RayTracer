#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "color.h"

// Common refraction indeces
namespace refraction_indeces {
    constexpr double Air   = 1.0;
    constexpr double Water = 1.33;
    constexpr double Glass = 1.5;
}

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
            const HitRecord& rec, 
            Color& attenuation, 
            Ray& scattered) const noexcept;
};

// A material to always scatter a ray
class Lambertian : public Material {
    public:
        Lambertian(const Color& albedo);

        bool scatter(
            const Ray& in_r, 
            const HitRecord& rec, 
            Color& out_attenuation, 
            Ray& out_scattered) const noexcept override;
    
    private:
        Color albedo;
};

// Reflects a ray
class Metal : public Material {
    public:
        Metal(const Color& albedo, double fuzz);

        // Fuzzy reflection
        bool scatter(
            const Ray& in_r, 
            const HitRecord& rec, 
            Color& out_attenuation, 
            Ray& out_scattered) const noexcept override;
        
    private:
        Color albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double refraction_index);

        // Refraction
        bool scatter(
            const Ray& in_r, 
            const HitRecord& rec, 
            Color& out_attenuation, 
            Ray& out_scattered) const noexcept override;
        
    private:
        double refraction_index;
        
        // Shlick's appromixation for reflectance
        double reflectance(double cosine, double ri) const noexcept;
};

#endif