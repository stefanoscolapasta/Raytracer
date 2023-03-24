#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility stuff
inline double degrees_to_radians(double degrees){
    return degrees * pi / 180.0;
}

inline double random_double() {
    //adding the +1 we ensure that rand in [0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max-min)*random_double(); //not includes max itself
}

inline double clamp(double x, double min, double max){
    if(x>max) return max;
    if(x<min) return min;
    return x;
}

//Common headers
#include "ray.h"
#include "vec3.h"

#endif