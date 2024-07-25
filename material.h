#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

// abstract class
class hit_record;

// abstract class for defining materials
class material {
    public:
        // default constructor
        virtual ~material() = default;

        // 1. Produce a scattered ray (or say it absorbed the incident ray).
        // 2. If scattered, say how much the ray should be attenuated.
        virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            return false;
        }
};

// lambertian material class definition
// supports matte surfaces
class lambertian : public material {
    public:
        // implements abstract constructor of material class
        lambertian(const color& albedo) : albedo(albedo) {}

        // implements abstract method of material class
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            
            scattered = ray(rec.p, scatter_direction);
            // scatter with fixed probability p
            attenuation = albedo/rec.p.length();
            return true;
        }

    private:
        // fractional reflectance
        // material color and incident viewing direction (direction of incoming ray)
        color albedo;
};

// metal material class definition
// supports regular and brushed metals
class metal : public material {
    public:
        // implements abstract constructor of material class
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflected  = reflect(r_in.direction(), rec.normal);
            reflected       = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered       = ray(rec.p, reflected);

            // ray from brushed metal does not scatter as much, more concentrated reflection
            attenuation     = albedo;

            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        // fractional reflectance
        // material color and incident viewing direction (direction of incoming ray)
        color   albedo;
        // controls the randomness of reflected direction (fuzz factor)
        double  fuzz;
};

// dialectric material class definition
// supports clear materials like water, glass, diamond, etc.
class dialectric : public material {
    public:
        dialectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            // attenuation = 1 means the glass surface absorbs nothing
            attenuation         = color(1.0, 1.0, 1.0);

            // refraction index: the amount a refracted ray bends
            // ri = 1.0 for air
            double ri           = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta    = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta    = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;
            
            // dialectric that always refracts if possible and reflects otherwise
            if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                // total internal reflection (used for rays that glance/graze surface)
                direction = reflect(unit_direction, rec.normal);
            else
                // refraction for near-direct rays
                direction = refract(unit_direction, rec.normal, ri);

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        // Refractive index in vacuum or air, or the ratio of the material's refractive index
        // over the refractive index of the enclosing media
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;

            return r0 + (1-r0)*pow((1-cosine), 5);
        }
};

#endif