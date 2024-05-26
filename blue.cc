/*  This code was referenced from "Ray Tracing in One Weekend"
*   @authors Peter Shirley, Trever David Black, Steve Hollasch
*/

#include <iostream>

int main() {

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // pixels are written in rows from left -> right and top -> bottom
    for (int j = 0; j < image_height; j++) {        // Columns
        for (int i = 0; i < image_width; i++) {     // Rows
            // rgb triplets (auto keyword declares local storage variables)
            auto r = double(i) / (image_width-1);
            auto g = double(image_width/2) / (image_width-1);
            auto b = double(j) / (image_width-1);

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}