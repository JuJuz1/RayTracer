#include "material.h"

#include <cmath>

#include "ray.h"
#include "hittable.h"
#include "color.h"
#include "vec3.h"
#include "rtweekend.h"

bool scatter(
    const Ray& in_r,
    const HitRecord& rec,
    Color& out_attenuation,
    Ray& out_scattered
) noexcept {
    MaterialType type{ rec.mat->type };

    switch (type) {
        case MaterialType::Lambertian: {
            auto* l = static_cast<LambertianData*>(rec.mat->data);
            Vec3 scatter_direction{ rec.normal + random_unit_vector() };
            if (scatter_direction.is_near_zero())
                scatter_direction = rec.normal;

            out_scattered = Ray{ rec.p, scatter_direction };
            out_attenuation = l->albedo;
            return true;
        }
        case MaterialType::Metal: {
            auto* m = static_cast<MetalData*>(rec.mat->data);
            Vec3 reflected{ reflect(in_r.direction(), rec.normal) };
            reflected = unit_vector(reflected) + (random_unit_vector() * m->fuzz);

            out_scattered = Ray{ rec.p, reflected };
            out_attenuation = m->albedo;
            return (dot(out_scattered.direction(), rec.normal) > 0);
        }
        case MaterialType::Dielectric: {
            auto* d = static_cast<DielectricData*>(rec.mat->data);
            out_attenuation = Colors::White;
            const double ri{ rec.front_face ? (1.0 / d->refraction_index) : d->refraction_index };

            const Vec3 unit_direction{ unit_vector(in_r.direction()) };
            double cos_theta{ std::fmin(dot(-unit_direction, rec.normal), 1.0) };
            double sin_theta{ std::sqrt(1.0 - cos_theta * cos_theta) };

            bool cannot_refract{ ri * sin_theta > 1};
            Vec3 direction;
            if (cannot_refract || reflectance(cos_theta, ri) > rt::random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, ri);

            out_scattered = Ray{ rec.p, direction};
            return true;
        }
        default:
            return false;
    }
}

double reflectance(double cosine, double ri) noexcept {
    double r0{ (1 - ri) / (1 + ri) };
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
