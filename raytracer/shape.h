#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "object.h"


class Material;

class Sphere : public Object {

    public:
        Sphere();
        Sphere(point3 center, double radius, Material* material);
        // Override the destructor.
        virtual ~Sphere() override;

        bool get_intersect(const Ray& ray, double t_min, double t_max, intersection& intersect);

        point3 position; // center of sphere
        double radius;
        Material* material;

};


class Quad : public Object
{
    public:
        Quad();
        Quad(point3 p1, point3 p2, point3 p3, Material* material);
        // Override the destructor.
        virtual ~Quad() override;

        // 3 points on the plane to compute the 2 vectors defining the plane
        point3 p1; // middle point connecting p2 and p3
        point3 p2;
        point3 p3;
        Material* material;

        // Override the function to test for intersections.
        bool get_intersect(const Ray& ray, double t_min, double t_max, intersection& intersect);
    
};

#endif