#include "material.h"
#include "vec3.h"

bool Material::scatter(
    const Ray& in_r, 
    const Hit_record& rec, 
    color& attenuation, 
    Ray& scattered) const noexcept {
        return false;
}

Lambertian::Lambertian(const color& albedo) : albedo(albedo) {};

bool Lambertian::scatter(
    const Ray& in_r, 
    const Hit_record& rec, 
    color& attenuation, 
    Ray& scattered) const noexcept {
        const Vec3 scatter_direction{ rec.normal + random_unit_vector() };
        scattered = Ray{ rec.p, scatter_direction };
        attenuation = albedo;
        return true;
}