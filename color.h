#ifndef COLOR_H // start of header file
#define COLOR_H // color class definition

// Import libraries

#include "interval.h"
#include "vec3.h"
#include "rtweekend.h"

using color = vec3; // vec3 color alias

// color utility functions

// converts linear color components to gamma 2
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return sqrt(linear_component);
    
    return 0;
}

// output multi-sample pixel color components
void write_color(std::ostream& out, const color& pixel_color) {
    // rgb triplet as vector positions
    // (auto keyword declares local storage variables)
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0, 255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif // end of header file