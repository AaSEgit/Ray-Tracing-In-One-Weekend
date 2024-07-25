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

    /*
    // create sphere objects
    auto material_ground    = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center    = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left      = make_shared<dialectric>(1.50);
    auto material_bubble    = make_shared<dialectric>(1.00 / 1.50);
    auto material_right     = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // add objects to the world scene
    world.add(make_shared<sphere>(point3( 0.0, - 100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    */

   // Test adjustable field-of-view (fov)
   auto R = cos(pi/4);

   auto material_left  = make_shared<lambertian>(color(0,0,1));
   auto material_right = make_shared<lambertian>(color(1,0,0));

   world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
   world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

    // Camera
    camera cam;

    cam.aspect_ratio        = 16.0 / 9.0;
    cam.image_width         = 400;
    cam.samples_per_pixel   = 100;
    cam.max_depth           = 50;

    cam.vfov = 90;  // orthographic wide-angle view

    // Render
    cam.render(world);
}