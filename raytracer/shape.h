#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "object.h"
#include "vec3.h"

class Sphere : public Object {

public:
    Sphere();
    Sphere(point3 center, double radius, std::shared_ptr<Material> material);
    // Override the destructor.
    virtual ~Sphere() override;

    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect);
    glm::dvec3 getNormal(const point3 point);

    point3 position;
    double radius;
    std::shared_ptr<Material> material_ptr;

   
};


class Quad : public Object
{
public:
    /* The default constructor.
        Note that this will define a unit sphere at the origin. */
    Quad();
    Quad(point3 p1, point3 p2, point3 p3, std::shared_ptr<Material> material);

    // Override the destructor.
    virtual ~Quad() override;

    // Override the function to test for intersections.
    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect);

    void rotateX(float degrees);


public:
    // position at center 
  
    // position = point3 p1; is the point on plane
    // 2 vectors defining the plane
    point3 p1;
    point3 p2;
    point3 p3;

    std::shared_ptr<Material> material_ptr;
};

//
//
//class Box : public Object
//{
//public:
//    /* The default constructor.
//        Note that this will define a unit sphere at the origin. */
//    Box();
//    Box(point3 center, double width, double height, double depth, std::shared_ptr<Material> material);
//
//    // Override the destructor.
//    virtual ~Box() override;
//
//    // Override the function to test for intersections.
//    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) ;
//
//public:
//    // position at center 
//    double width;
//    double height;
//    double depth;
//    std::shared_ptr<Material> material_ptr;
//};
//
//class Triangle : public Object {
//
//public:
//    // triangle can be defined by three points and its normal
//    Triangle() {}
//    Triangle(point3 p1, point3 p2, point3 p3, std::shared_ptr<Material> material);
//    bool getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect);
//    glm::dvec3 getNormal();
//
//    point3 p1;
//    point3 p2;
//    point3 p3;
//
//    std::shared_ptr<Material> material_ptr;
//};
//
//class Quad : public Object {
//
//public:
//    // triangle can be defined by three points and its normal
//    Quad() {}
//    Quad(point3 p1, point3 p2, point3 p3, std::shared_ptr<Material> material);
//    bool getIntersect(const ray& ray, double t_min, double t_max, intersection& intersect);
//    glm::dvec3 getNormal();
//
//    point3 p1;
//    point3 p2;
//    point3 p3;
//
//    std::shared_ptr<Material> material_ptr;
//};


//class Cube : public Triangle {
//
//public:
//    struct triangle_data {
//        point3 p1;
//        point3 p2;
//        point3 p3;
//    };
//    // triangle can be defined by three points and its normal
//    Cube() {}
//    Cube(point3 center, double length, std::shared_ptr<Material> material);
//    bool getIntersect(const ray& ray, double t_min, double t_max, intersection& intersect);
//
//    double length;
//    point3 p1; 
//    point3 p2;
//    point3 p3;
//    point3 p4;
//    point3 p5;
//    point3 p6;
//    point3 p7;
//    point3 p8;
//    std::vector<triangle_data> triangle_list;
//};

#endif