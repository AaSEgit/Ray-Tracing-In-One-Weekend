#ifndef CAMERA_H    // start of camera header file
#define CAMERA_H    // camera class defintition

// Import libraries
#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

// 1. Constructs and dispatches rays into the world
// 2. Uses the results of these rays to construct a rendered image
class camera {
    public:
        /* Public Camera Parameters*/
        double  aspect_ratio        = 1.0;  // Ratio of image width over height
        int     image_width         = 100;  // Rendered image width in pixel count
        int     samples_per_pixel   = 10;   // Count of random samples for each pixel
        int     max_depth           = 10;   // Maximum number of ray bounces into scene

        double  vfov        = 90;               // Vertical view angle (field of view)
        point3  lookfrom    = point3(0,0,0);    // Point camera is looking from
        point3  lookat      = point3(0,0,-1);   // Point camera is looking at
        vec3    vup         = vec3(0,1,0);      // Camera-relative "up" direction
        
        // Render an image
        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            // pixels are written in rows from left -> right and top -> bottom
            for (int j = 0; j < image_height; j++) {        // Rows
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {     // Columns
                // additive pixel color
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        // define pixel color
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    // write out color components
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }

            std::clog << "\rDone.               \n";
        }

        // return color for a given scene ray
        color ray_color(const ray& r, int depth, const hittable& world) {
            // If we've exceeded the ray bounce limit, no more light is gathered
            // return black outside depth limit
            if (depth <= 0) {
                return color(0,0,0);
            }

            hit_record rec;

            // ignores hits close to the estimated intersection point
            // calculating reflected ray origins with tolerance
            if (world.hit(r,interval(0.001, infinity), rec)) {
                // ray color is affected by material information
                ray scattered;
                color attenuation;
                if(rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                // return matte gray (color is affected by ambient light)
                return color(0,0,0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            // blendedValue = (1-a)*startValue + a*endValue
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }

    private:
        /* Private Camera Paramters*/
        int     image_height;           // Rendered image height
        double  pixel_samples_scale;    // Color scale factor for a sum of pixel samples
        point3  center;                 // Camera center
        point3  pixel00_loc;            // Location of pixel 0, 0
        vec3    pixel_delta_u;          // Offset to pixel to the right
        vec3    pixel_delta_v;          // Offset to pixel below
        vec3    u,v,w;                  // Camera frame basis vectors

        void initialize() {
            // Calculate the image height, and ensure that it's at least 1
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            // Determine viewport dimensions
            auto focal_length    = (lookfrom - lookat).length();
            auto theta           = degrees_to_radians(vfov); // vertical field of view in radians
            auto h               = tan(theta/2);             // y-plane opposite vfov (intersects z-plane)
            auto viewport_height = 2 * h * focal_length;     // y-plane opposite vfov (full-length)
            // Viewport widths less than one are ok since they are real valued
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the u,v,w unit basis vectors for the camera coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w,u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges
            vec3 viewport_u = viewport_width * u;   // Vector across viewport horizontal edge
            vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

            // Calculate the horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel
            auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled
            // point around the pixel location i, j

            auto offset = sample_square();
            // formerly pixel center
            auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        // Returns the vector to a random point in the [-.5, -.5]-[+.5, +.5] unit square
        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        // uniform diffuse
        vec3 uniform_diffuse(vec3 normal) const {
            return random_on_hemisphere(normal);
        }

        // replacement diffuse with non-uniform Lambertian distribution
        vec3 replacement_diffuse(vec3 normal) const {
            return normal + random_unit_vector();
        }
};

#endif  // end of camera header file