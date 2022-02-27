#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "utility.h"
#include "vec3.h"
#include <iostream>

class Material;
class Object;
// make this a function later? in shapeobject that takes in intersection, normal, tangent etc to calculate  how light bounces off


struct intersection {
    Object* object;
    point3 point;
    // normals always point against ray (so if ray is hitting from inside, normal will point in
    glm::dvec3 normal;
    double t;
    bool front_face;
    //material pointer to find out what ray, if any, is scattered
    //A reference must be initialized on declaration while it is not necessary in case of pointer
    // both share memory address, shared_ptr can be empty
    Material* material;
    // &mat to get address , *mat to get value

    inline void set_face_normal(const Ray& r, const glm::dvec3& outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


// abstract class , but is light an object? we don't need its normal
class Object {
public:
    // Constructor and destructor.
    Object();
    virtual ~Object();
      
    // abstract needs to be redefined
    virtual bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) = 0;

};


#endif