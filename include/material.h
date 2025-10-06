#ifndef INCLUDE_MATERIAL_H_
#define INCLUDE_MATERIAL_H_

#include "ray.h"
#include "hittable.h"
#include "color.h"

// Needed for scatter
struct HitRecord;

// Common refraction indeces
namespace refraction_indeces {

constexpr double Air   { 1.0  };
constexpr double Water { 1.33 };
constexpr double Glass { 1.5  };

} // namespace refraction_indeces

enum class MaterialType {
    Lambertian,
    Metal,
    Dielectric,

    MaxCount
};

struct Material {
    MaterialType type;
    void* data;
};

struct LambertianData {
    Color albedo;
};

struct MetalData {
    Color albedo;
    double fuzz;
};

struct DielectricData {
    double refraction_index;
};

// Scatter function for every material
bool scatter(
    const Ray& in_r,
    const HitRecord& rec,
    Color& out_attenuation,
    Ray& out_scattered) noexcept;

// Shlick's appromixation for reflectance
double reflectance(double cosine, double ri) noexcept;

#endif // INCLUDE_MATERIAL_H_
