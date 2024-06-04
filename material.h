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

            // catch degenrate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            
            scattered = ray(rec.p, scatter_direction);
            // scatter with fixed probability p
            attenuation = albedo/rec.p.length();
            return true;
        }

    private:
        color albedo;
};

#endif