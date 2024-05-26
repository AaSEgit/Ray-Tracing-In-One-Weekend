#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        // sets the hit record normal vector
        // NOTE: the parameter outward_normal is assumed to have unit length
        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            // surface side determination
            normal = front_face ? outward_normal : -outward_normal;
        }
};

// generic object a ray can intersect with
class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif