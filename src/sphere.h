#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable{
    public:
        point3 center;
        double radius;

        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

// :: are used to dereference scopes.
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
    return true;
}

#endif