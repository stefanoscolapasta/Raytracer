#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable{
    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;

        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override; //this is a declaration of the virtual func
};

// :: are used to dereference scopes.
//this is the definition of the overriden virtual func
//This definition could also be placed in the class to have declaration and definition in the same place
//done like this to keep the class itself more readable, if function is small consider defining it inside
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); //Because it's equivalent to dot product of vector for itself
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    
    auto discriminant = half_b*half_b - a*c;
    
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    //Now I need to find the nearest root that lies inside the given (t_min,t_max) range
    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || root > t_max){
        root = (-half_b + sqrtd) / a;
        if(root < t_min || root > t_max) return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = unit_vector(rec.p - center);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

#endif