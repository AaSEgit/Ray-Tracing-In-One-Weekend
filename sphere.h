#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "rtweekend.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius) : center(center), radius(fmax(0, radius)) {
            // TODO: Initialize the material pointer 'mat'
        }

        // determines if a ray intersects with a sphere
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            // discriminant of quadratic equation
            auto discriminant = h*h - a*c;
            // no solution, ray does not intersect with the sphere
            if (discriminant < 0)
                return false;
            
            // square root of the discriminant
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            // surface side determination
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
};

#endif