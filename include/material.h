#ifndef INCLUDE_MATERIAL_H_
#define INCLUDE_MATERIAL_H_

#include <optional>

#include "ray.h"
#include "hittable.h"
#include "color.h"

// Common refraction indeces
namespace refraction_indeces {

constexpr double Air   { 1.0  };
constexpr double Water { 1.33 };
constexpr double Glass { 1.5  };

} // namespace refraction_indeces

struct ScatterRecord {
    Ray scattered;
    Color attenuation;
};

// A material class to:
// 1. Produce a scattered ray
// 2. If scattered tell how much the ray should be attenuated (colored)
class Material {
 public:
    virtual ~Material() = default;

    // Every hittable object needs a material so we will
    // use this base class as the material and just return false
    [[nodiscard]] virtual std::optional<ScatterRecord>
    scatter(const Ray& r, const HitRecord& rec) const noexcept;
};

// A material to always scatter a ray
class Lambertian : public Material {
 public:
    explicit Lambertian(const Color& albedo);

    [[nodiscard]] std::optional<ScatterRecord>
    scatter(const Ray& r, const HitRecord& rec) const noexcept override;

 private:
    Color albedo;
};

// Reflects a ray
class Metal : public Material {
 public:
    Metal(const Color& albedo, double fuzz);

    // Fuzzy reflection
    [[nodiscard]] std::optional<ScatterRecord>
    scatter(const Ray& r, const HitRecord& rec) const noexcept override;

 private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
 public:
    explicit Dielectric(double refraction_index);

    // Refraction
    [[nodiscard]] std::optional<ScatterRecord>
    scatter(const Ray& r, const HitRecord& rec) const noexcept override;

 private:
    double refraction_index;

    // Shlick's appromixation for reflectance
    [[nodiscard]] double reflectance(double cosine, double ri) const noexcept;
};

#endif // INCLUDE_MATERIAL_H_
