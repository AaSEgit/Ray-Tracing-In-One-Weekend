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
class metal : public material {
    public:
        // implements abstract constructor of material class
        metal(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected);
            // ray from brushed metal does not scatter as much, more concentrated reflection
            attenuation = albedo;
            return true;
        }

    private:
        // fractional reflectance
        // material color and incident viewing direction (direction of incoming ray)
        color albedo;


};

#endif