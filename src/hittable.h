#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

//used to register information on every single hit found in the world
struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face; //It's to know if ray is facing same direction of surface or not

    inline void set_face_normal(const ray& r, const vec3& outward_normal){
        //remember to think of it when connecting both vectors by initial point, don't get confused
        //It's the opposite of what it might look like
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        //Putting const after a member function indicates that the code inside it will not modify the containing object (eg. cant change variables) 
        //the "=0" instead is related to virtual, not to const, and it indicates that the function is pure virtual, must be overriden by a sub-class
        //classes that have pure virtual functions are abstract, like this hittable, cant define a hittable object directly, need a sub-class
};

#endif