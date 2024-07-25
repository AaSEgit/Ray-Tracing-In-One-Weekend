#ifndef RAY_H   // start of header file
#define RAY_H   // ray class definition

// Import libraries
#include "vec3.h"

class ray {
    public:
        // constructors
        ray() {}

        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        // accessor methods for returning origin and direction values
        const point3& origin() const {return orig;}
        const vec3& direction() const {return dir;}

        // returns the value of a point along a ray
        point3 at(double t) const {
            return orig + t*dir;
        }

    private:
        point3  orig;
        vec3    dir;

};

#endif  // end of header file