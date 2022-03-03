#pragma once
#ifndef RAY_H
#define RAY_H


class Ray {

    public:
        Ray() {}
        Ray(const point3& origin, const glm::dvec3& direction)
            : orig(origin), dir(direction)
        {}

        point3 orig; // ray origin
        glm::dvec3 dir; // ray direction (unit length)

        // get functions (const to not accidentally edit)
        point3 origin() const { return orig; }
        glm::dvec3 direction() const { return dir; }

        // ray as a function P(t)=A+tb
        // P is a 3D position along a line in 3D. A is the ray origin and b is the ray direction
        // different t and P(t) moves the point along the ray
        point3 at(double t) const {
            return orig + t * dir;
        }
        
};




#endif