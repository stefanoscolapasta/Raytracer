#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
//std::shared_ptr is a smart pointer that retains shared ownership of an object through a pointer. Meaning that
//It's a smart pointer designed for scenarios in which more than one owner might have to manage the lifetime of the object in memory.
//Several shared_ptr objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens:
// - the last remaining shared_ptr owning the object is destroyed;
// - the last remaining shared_ptr owning the object is assigned another pointer via operator= or reset()
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object){ add(object); } //Shared instance, gets automatically destroyed when last owner is destroyed

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for(const auto& object : objects){
        if(object->hit(r, t_min, closest_so_far, temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t; //Smart, no need to sort or check if it is the closest this way, closest is used as t_max
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif