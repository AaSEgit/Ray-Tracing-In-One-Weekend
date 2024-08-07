#ifndef VEC3_H  // start of vec3 header file
#define VEC3_H  // vec3 class definition

// Import libraries
#include "rtweekend.h"

// C++ Std Usings
using std::sqrt;

class vec3 {
    public:
        double e[3];    // 3D vector attribute

        // Constructors
        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        // Accessor methods
        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        // negation
        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);} 

        // vector access
        double operator[](int i) const {return e[i];} 

        // vector access by reference
        double& operator[](int i) {return e[i];}    

        // addition assignment
        vec3& operator+=(const vec3& v) {   
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        // mulitplication assignment
        vec3& operator*=(double t) {    
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        // division assignment
        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        // returns the length of a vector
        double length() const {
            return sqrt(length_squared());
        }

        // returns the squared length of a vector
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        // returns true if the vector is very close to 0 in all dimensions
        bool near_zero() const {
            auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

        // default constructor for generating random vector
        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        // generates random vector with a defined min and max component values
        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code
using point3 = vec3;

// Vector Utility Functions

//output vector
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// vector addition
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// vector subtraction
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// vector multiplication
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// scalar-vector multiplication
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// vector-scalar multiplication
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

// vector-scalar division
inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

// dot product
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

// cross product
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[2] * v.e[2],
                u.e[0] * v.e[1] - u.e[2] * v.e[0]);
}

// // creates a normal unit vector perpendicular to the surface of a unit sphere at that point
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

// generates a random unit vector using a rejection method
// 1. Generate a random vector inside of the unit sphere (center to surface)
// 2. Normalize this vector (clamp its value to the surface)
// 3. Invert the normalized vector if it falls onto the wrong hemisphere
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1 , 1);
        if (p.length_squared() < 1)
            return p;
    }
}

// normalizes a random vector to a unit vector on the surface of a unit sphere
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// inverts a normalized vector if it falls onto the wrong hemisphere
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)  // in the same hemipshere as normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

// ray reflection (ray bounces off surface material)
inline vec3 reflect(const vec3& v, const vec3& n) {
    // 1. ray = v + 2b
    // 2. b is the projection of v onto unit vector n = v*n
    // 3. if n is not a unit vector, divide dot product by length of n
    // 4. negate projection length so it points out of the surface
    return v - 2*dot(v,n)*n;
}

// ray refraction (ray is partially absorbed into surface material at an angle)
// Uses Snell's Law - etai*sin(theta) = etat*sin(theta-prime)
inline vec3 refract(const vec3& uv, const vec3& n ,double etai_over_etat) {
    // given two unit vectors, dot product between ray uv and surface normal n 
    auto cos_theta      = fmin(dot(-uv, n), 1.0);

    // part of ray that is perpendicular to the surface normal
    vec3 r_out_perp     = etai_over_etat * (uv + cos_theta*n);
    
    // part of ray that is parallel to the surface normal
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;

    // return refracted ray
    return r_out_perp + r_out_parallel;
}

#endif  // end of vec3 header file