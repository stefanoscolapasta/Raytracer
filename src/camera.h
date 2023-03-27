#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera{
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov, //vertical field-of-view in degrees
            double aspect_ratio
        ){ //now the camera has adjustable field of view
            //I think it would be more convienient to have a 
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            
            auto w = unit_vector(lookfrom - lookat); //it's because the camera faces -z :)
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            auto focal_length = 1.0;

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;
        }

        ray get_ray(double s, double t) const {
            vec3 direction = lower_left_corner + s*horizontal + t*vertical - origin;
            return ray(origin, direction);
        }
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif