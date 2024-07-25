#ifndef HITTABLE_H  // start of hittable header file
#define HITTABLE_H  // hittable class definition

// Import libraries
#include "rtweekend.h"

// abstract class
class material;

// contains information about the material assigned to the surface of 
// an object that was hit by a ray
class hit_record {
    public:
        point3                  p;
        vec3                    normal;
        shared_ptr<material>    mat;   // material pointer
        double                  t;
        bool                    front_face;

        // sets the hit record normal vector
        // NOTE: the parameter outward_normal is assumed to have unit length
        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face  = dot(r.direction(), outward_normal) < 0;
            // surface side determination
            normal      = front_face ? outward_normal : -outward_normal;
        }
};

// generic object a ray can intersect with
class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif  // end of hittable header file