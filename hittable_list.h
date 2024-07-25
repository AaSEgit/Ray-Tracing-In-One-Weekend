#ifndef HITTABLE_LIST_H // start of hittable_list header file
#define HITTABLE_LIST_H // hittable_list class definition


// Import libraries
#include "hittable.h"
#include "rtweekend.h"
#include <vector>

// stores a list of hittable objects
class hittable_list : public hittable {
    public:
        // vector/list that stores hittable objects
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) {add(object);}

        // empties the contents of a hittable_list
        void clear() {objects.clear();}

        // adds a hittable object to a hittable_list
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        // returns true if a ray intersects with any object in the hittable_list 
        // within an acceptable range
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record  temp_rec;
            bool        hit_anything    = false;
            auto        closest_so_far  = ray_t.max;

            for (const auto& object : objects) {
                if (object->hit(r,  interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything    = true;
                    closest_so_far  = temp_rec.t;
                    rec             = temp_rec;
                }
            }

            return hit_anything;
        }
};

#endif  // end of hittable_list header file