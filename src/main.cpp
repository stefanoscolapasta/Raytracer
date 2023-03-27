#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>

//With this we are calculating whether a ray intersects with a sphere that has a certain radius and center(not only (0,0,0))
//It's pretty simple to check for intersection by checking for the discriminant of the obtained quadratic equation
double hit_sphere(const point3& center, double radius, const ray& r){
    //We implemented some simplifications to make it slightly faster
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); //Because it's equivalent to dot product of vector for itself
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

//I passed hittable cause hittable_list is sub-class of hittable
color ray_color(const ray& r, const hittable& world, int depth){
    hit_record rec;

    //We need to limit the ray bounce depth, meaning that no more light is gathered if limit is reached

    if (depth <= 0)
        return color(0,0,0); //means we will not be gathering more light

    //0.001 to solve the shadow acne problem to make for floating point approximation, this actually solves EVERYTHING
    //TODO: investigate further HOW changing the t_min from 0 to 0.001 totally changes the brightness outcome
    if (world.hit(r, 0.001, infinity, rec)){
        //What we are doing here in simple is: we have the point of intersection, we sum the normal to get to the center of the unit sphere
        //then we generate this random IN unit vector (not unit vector!). This means that now we obtain a random point that lies inside
        //the tangent sphere to the intersection point with center obtained with the normal, this is used for diffuse random scattering
        //to obtain rando point we switched from random_in_unit_sphere to random_unit_vector
        //This makes the objects appear lighter because fewer rays are scattered toward the normal --> more are scattered toward the camera
        //This decreases also the amount of light that gets trapped underneath objects, because as said before, less rays are scattered straight up
        //As you remember, color of a point in this kind of material is gained by surroundings, if a light ray gets trapped it gets darker and darker
        //In this case shadows are created for the vicinity of objects, not because there is a true light source.
        
        //OPTION1-> point3 target = rec.p + rec.normal + random_in_unit_sphere();
        //OPTION2-> point3 target = rec.p + rec.normal + random_unit_vector();
        
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y()+1.0); //This is for the background
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1920;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    //World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);
    //Render
    
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0); //we start at 0,0,0 and add up single contributions from each sample taken
            for(int s=0; s < samples_per_pixel; ++s){
                //This way we get the percentage at which we are for each direction, z is constant so no prob
                //So not based on single pixel but position along whole width
                auto u = (i+random_double()) / (image_width - 1);
                auto v = (j+random_double()) / (image_height - 1);

                //Basically to get the direction vector for each pixel, we offset by the 
                //horizontal and vertical vectors for the percentage at which they are, subtracting by the origin gives us the vector
                //Remember: pos-pos gives us the result vector, with direction towards the pixel
                
                ray r = cam.get_ray(u,v);
                //Summing up for later averaging for anti-aliasing
                pixel_color += ray_color(r, world, max_depth); //the value is a set of values that can be in range (0,..,100;0,..,100;0,..,100)
                //So that's why in write color we scale it down by the number of samples (100 in this case)
            }

            write_color(std::cout, pixel_color, samples_per_pixel); 
        }
    }
}