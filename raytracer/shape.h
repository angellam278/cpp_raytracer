#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "object.h"
#include "vec3.h"

class Material;
class Sphere : public Object {

public:
    Sphere();
    // radius to float?? 
    Sphere(point3 center, double radius, Material* material);
    // Override the destructor.
    virtual ~Sphere() override;

    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect);
    glm::dvec3 getNormal(const point3 point);

    point3 position;
    double radius;


    Material* material;
};


class Quad : public Object
{
public:
    /* The default constructor.
        Note that this will define a unit sphere at the origin. */
    Quad();
    Quad(point3 p1, point3 p2, point3 p3, Material* material);

    // Override the destructor.
    virtual ~Quad() override;

    // Override the function to test for intersections.
    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect);


public:
    // position at center 
  
    // position = point3 p1; is the point on plane
    // 2 vectors defining the plane
    point3 p1;
    point3 p2;
    point3 p3;
    Material* material;
};

#endif