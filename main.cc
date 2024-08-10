/*  This code was referenced from "Ray Tracing in One Weekend"
*   @authors Peter Shirley, Trever David Black, Steve Hollasch
*/

// Import libraries
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
    // World
    hittable_list world;

    // create and add ground material to world scene
    auto material_ground    = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, material_ground));

    // generate random spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            // random value to choose a material
            auto choose_mat = random_double();
            // random position in the viewport
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dialectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // create and add 3 large spheres to the world scene
    auto material1 = make_shared<dialectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Camera
    camera cam;

    cam.aspect_ratio        = 16.0 / 9.0;
    cam.image_width         = 240;
    cam.samples_per_pixel   = 10;
    cam.max_depth           = 50;

    cam.vfov        = 20;  // wide-angle view
    cam.lookfrom    = point3(13,2,3);
    cam.lookat      = point3(0,0,0);
    cam.vup         = vec3(0,1,0);
    
    // depth-of-field (defocus blur)
    cam.defocus_angle   = 0.6;
    cam.focus_dist      = 10.0;

    // Render
    cam.render(world);
}