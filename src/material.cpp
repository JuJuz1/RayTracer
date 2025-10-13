#include "material.h"

#include <optional>
#include <cmath>

#include "ray.h"
#include "hittable.h"
#include "color.h"
#include "vec3.h"
#include "rtweekend.h"

std::optional<ScatterRecord> Material::scatter(
    const Ray& r,
    const HitRecord& rec
) const noexcept {
    return {};
}

Lambertian::Lambertian(const Color& albedo) : albedo{ albedo } {};

std::optional<ScatterRecord> Lambertian::scatter(
    const Ray& r,
    const HitRecord& rec
) const noexcept {
    Vec3 scatter_direction{ rec.normal + random_unit_vector() };
    if (scatter_direction.is_near_zero())
        scatter_direction = rec.normal;

    return ScatterRecord{ Ray{ rec.p, scatter_direction }, albedo };
}

Metal::Metal(const Color& albedo, double fuzz) : albedo{ albedo }, fuzz{ std::fmax(fuzz, 0.0) } {};

std::optional<ScatterRecord> Metal::scatter(
    const Ray& r,
    const HitRecord& rec
) const noexcept {
    Vec3 reflected{ reflect(r.direction(), rec.normal) };
    reflected = unit_vector(reflected) + (random_unit_vector() * fuzz);

    const Ray scattered{ rec.p, reflected };
    if (0 < dot(scattered.direction(), rec.normal))
        return ScatterRecord{ scattered, albedo };

    return {};
}

Dielectric::Dielectric(double refraction_index) : refraction_index{ refraction_index} {};

std::optional<ScatterRecord> Dielectric::scatter(
    const Ray& r,
    const HitRecord& rec
) const noexcept {
    const double ri{ rec.front_face ? (1.0 / refraction_index) : refraction_index };

    const Vec3 unit_direction{ unit_vector(r.direction()) };
    double cos_theta{ std::fmin(dot(-unit_direction, rec.normal), 1.0) };
    double sin_theta{ std::sqrt(1.0 - cos_theta * cos_theta) };

    const bool cannot_refract{ 1 < ri * sin_theta };
    Vec3 direction;
    if (cannot_refract || rt::random_double() < reflectance(cos_theta, ri))
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, ri);

    return ScatterRecord{ Ray{ rec.p, direction}, colors::White };
}

double Dielectric::reflectance(double cosine, double ri) const noexcept {
    double r0{ (1 - ri) / (1 + ri) };
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
