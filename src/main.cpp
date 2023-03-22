#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

//With this we are calculating whether a ray intersects with a sphere that has a certain radius and center(not only (0,0,0))
//It's pretty simple to check for intersection by checking for the discriminant of the obtained quadratic equation
double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}

color ray_color(const ray& r){
    auto t = hit_sphere(point3(0,0, -1), 0.5, r);
    
    if (t > 0.0) {
        //Basically by doing intersection - center we get the normal
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        t = 0.5*(unit_direction.y() + 1.0); //We add 1.0 to the y component because to interpolate we need values between 0 and 1, also, multiply by 0.5 to reduce domain, otherwise it would be 0,...,2
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    }
}

int main() {
    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    
    //Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);
    //Render
    
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {

            //This way we get the percentage at which we are for each direction, z is constant so no prob
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            
            //Basically to get the direction vector for each pixel, we offset by the 
            //horizontal and vertical vectors for the percentage at which they are, subtracting by the origin gives us the vector
            //Remember: pos-pos gives us the result vector, with direction towards the pixel
            vec3 direction = lower_left_corner + u*horizontal + v*vertical - origin;

            ray r(origin, direction);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
}