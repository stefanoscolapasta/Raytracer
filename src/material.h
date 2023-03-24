#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
    public:
    //this abstract class serves as blueprint for all materials. Every material gets an incoming ray, needs to store the hit information, has a specific attenuation and scatters the ray
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const=0;
};

class lambertian : public material{
    public:
        color albedo;
        lambertian(const color& a) : albedo(a){}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            auto scatter_direction = rec.normal + random_unit_vector();

            //if random_unit_vector == -rec.normal then they sum up to 0, and this can cause problems :)
            if(scatter_direction.near_zero())
                scatter_direction = rec.normal; //does this not lead in having a higher prob of light being scattered towards the normal direction?

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material{
    public:
        color albedo;
        metal(const color& a) : albedo(a){}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0); //I do not understand the necessity for this check
        }
};

#endif