#include <cmath>

#include "material.h"
#include "vec3.h"
#include "rtweekend.h"

bool Material::scatter(
    const Ray& in_r, 
    const Hit_record& rec, 
    color& attenuation, 
    Ray& scattered) const noexcept {
        return false;
}

Lambertian::Lambertian(const color& albedo) : albedo{ albedo } {};

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

Metal::Metal(const color& albedo, double fuzz) : albedo{ albedo }, fuzz{ std::fmax(fuzz, 0.0) } {};

bool Metal::scatter(
    const Ray& in_r, 
    const Hit_record& rec, 
    color& out_attenuation, 
    Ray& out_scattered) const noexcept {
        Vec3 reflected{ reflect(in_r.direction(), rec.normal) };
        reflected = unit_vector(reflected) + (random_unit_vector() * fuzz);
        out_scattered = Ray{ rec.p, reflected };
        out_attenuation = albedo;
        return (dot(out_scattered.direction(), rec.normal) > 0);
}

Dielectric::Dielectric(double refraction_index) : refraction_index{ refraction_index} {};

bool Dielectric::scatter(
    const Ray& in_r, 
    const Hit_record& rec, 
    color& out_attenuation, 
    Ray& out_scattered) const noexcept {
        out_attenuation = Color::White;
        const double ri{ rec.front_face ? (1.0 / refraction_index) : refraction_index };

        const Vec3 unit_direction{ unit_vector(in_r.direction()) };
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract{ ri * sin_theta > 1};
        Vec3 direction;
        if (cannot_refract || reflectance(cos_theta, ri) > rt::random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        out_scattered = Ray{ rec.p, direction};
        return true;
}

double Dielectric::reflectance(double cosine, double refraction_index) noexcept {
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}