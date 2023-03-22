#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:

        point3 orig;
        vec3 dir;

        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction) //This is just a more efficient way of initializing orig=origin and dir=direction, it is known as member initialization list
            //maybe a reason is also that being origin and directio are const and references, both of which cannot be assigned
        {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }

        point3 at(double t) const {
            return orig + t*dir;
        }
};

#endif