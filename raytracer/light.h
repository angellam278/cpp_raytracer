#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "object.h"
#include "vec3.h"

class Light {
public:
    /* The default constructor.
        Note that this will define a unit sphere at the origin. */
    Light();
    Light(point3 position, color diffuse, color specular);

    // Override the destructor.
    //virtual ~Light() override;

    // Override the function to test for intersections.
    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect);


public:
    // position at center 

    // position = point3 p1; is the point on plane
    // 2 vectors defining the plane
    point3 position;
    color diffuse;
    color specular;
    //color color;
    //double intensity = 1.0;
};

#endif LIGHT_H