#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "utility.h"
#include <iostream>


// forward declaration
class Material;
class Object;

/*
* intersection struct stores information about an intersection of object and ray
*/
struct intersection {
    // object that ray intersects with
    Object* object; 
    // point of intersection
    point3 point;
    // outward normal at intersection (always point against ray)
    glm::dvec3 normal; 
    // t along ray at intersection
    double t; 
    // material at intersection surface (to tell how light will react)
    Material* material; 
    bool front_face;

    // function that makes sure normal is always again the ray
    inline void set_face_normal(const Ray& r, const glm::dvec3& outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


// abstract class 
class Object {
    public:
        // Constructor and destructor.
        Object();
        virtual ~Object();
      
        // abstract needs to be redefined
        virtual bool get_intersect(const Ray& ray, double t_min, double t_max, intersection& intersect) = 0;
};


#endif