#pragma once
#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray() {}
        Ray(const point3& origin, const glm::dvec3& direction)
            : orig(origin), dir(direction)
        {}

        point3 origin() const { return orig; }
        // ray direction is unit length (vector)
        glm::dvec3 direction() const { return dir; }

        //ray as a function P(t)=A+tb
        //P  is a 3D position along a line in 3D. A is the ray origin and b is the ray direction
        //different t and P(t) moves the point along the ray
        point3 at(double t) const {
            return orig + t * dir;
        }

    public:

        point3 orig;
        glm::dvec3 dir;


};




#endif