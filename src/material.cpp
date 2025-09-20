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
    color& out_attenuation, 
    Ray& out_scattered) const noexcept {
        Vec3 scatter_direction{ rec.normal + random_unit_vector() };
        if (scatter_direction.is_near_zero())
            scatter_direction = rec.normal;
        
        out_scattered = Ray{ rec.p, scatter_direction };
        out_attenuation = albedo;
        return true;
}

Metal::Metal(const color& albedo) : albedo(albedo) {};

bool Metal::scatter(
    const Ray& in_r, 
    const Hit_record& rec, 
    color& out_attenuation, 
    Ray& out_scattered) const noexcept {
        Vec3 reflected{ reflect(in_r.direction(), rec.normal) };
        out_scattered = Ray{ rec.p, reflected };
        out_attenuation = albedo;
        return true;
}