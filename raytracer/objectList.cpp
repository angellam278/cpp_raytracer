#include "objectList.h"


bool ObjectList::getIntersect(const Ray& r, double t_min, double t_max, intersection& intersect) const {
    intersection temp_intersect;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // auto so can be any object
    for (const auto& object : objects) {
        if (object->getIntersect(r, t_min, closest_so_far, temp_intersect)) {
            hit_anything = true;
            closest_so_far = temp_intersect.t;
            intersect = temp_intersect;
        }
    }

    return hit_anything;
}
