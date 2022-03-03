#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "object.h"


class Light {
        
    public:
        Light();
        ~Light();
        Light(point3 position, color diffuse, color specular);

        point3 position;
        color diffuse;
        color specular;

        // Override the function to test for intersections.
        bool get_intersect(const Ray& ray, double t_min, double t_max, intersection& intersect);

};

#endif LIGHT_H