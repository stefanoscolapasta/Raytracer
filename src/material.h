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
        double fuzz;

        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            //fuzz is a scalar, we multiply fuzz by a randon in unit vector to scale it's magnitude and reduce the fuzziness effect
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0); //I do not understand the necessity for this check
        }
};

class dielectric : public material {
    //This only refracts
    public:

        double ir;

        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            //A thing to keep in mind is that not every ray can be refracted, for some angles and refraction indices, no solution exists, and ray should be reflected
            attenuation = color(1.0, 1.0, 1.0); //Meaning that it absorbs nothing (to simulate glass)
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir; //1/ir because if it comes from outside then ir of air is 1

            vec3 unit_direction = unit_vector(r_in.direction());

            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1-cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        static double reflectance(double cosine, double ref_idx){
            //Using Schlick's approximation for reflectance
            //Here 1 is used because one of the interfaces is usually air
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0*r0;
            //This formula return the reflection coefficient
            //The reflection coefficient is a parameter that describes how much of a wave is reflected by an impedance discontinuity in the transmission medium.
            return r0 + (1-r0)*pow((1-cosine), 5);
        }
};

#endif