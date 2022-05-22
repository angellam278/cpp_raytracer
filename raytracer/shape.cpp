
#include "shape.h"

// default constructor
Sphere::Sphere() {}
// destructor
Sphere::~Sphere() {}

Sphere::Sphere(point3 center, double radius, Material* material) {
    this->position = center;
    this->radius = radius;
    this->material = material;
}

/*
 Sphere intersection
  - solving equation for a point on ray which is at distance r from center and stores info in intersect
*/
bool Sphere::get_intersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {

    // (p-c).(p-c) = r^2 where p = point on sphere, c = center of sphere and r is the radius
    // p = ray_origin + t(ray_direction), we are trying to find the t in which the ray intersects the sphere
    // --> ((ray_origin + t(ray_direction)) - c).((ray_origin + t(ray_direction)) - c) = r^2
    // --> ((ray_origin - c) + t(ray_direction)).((ray_origin - c) + t(ray_direction)) = r^2
    // -- > (origin_center + t(ray_direction)).(origin_center + t(ray_direction)) = r^2
    // -- > (ray_direction.ray_direction)t^2 + 2(origin_center.ray_direction)t + (origin_center.origin_center - r^2) = 0
    // -- > at^2 + 2bt + c = 0

    // vector from sphere to ray origin
    glm::dvec3 oc = ray.origin() - position;
    // length2 = equivalent to dot product
    double a = glm::length2(ray.direction());
    double b = 2 * glm::dot(oc, ray.direction());
    double c = glm::length2(oc) - (radius * radius);

    double discriminant = (b * b) - 4 * a * c;
    if (discriminant < 0) { return false; }
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range. 
    double root = (-b - sqrtd) / 2 * a;
    if (root < t_min || t_max < root) {
        root = (-b + sqrtd) / 2 * a;
        if (root < t_min || t_max < root)
            return false;
    }

    // record intersection info:
    intersect.t = root;
    // point of intersection in global coordinate
    intersect.point = ray.at(intersect.t);
    // normalized normal
    glm::dvec3 outward_normal = (intersect.point - position) / radius;
    intersect.set_face_normal(ray, outward_normal);
    intersect.material = material;
    intersect.object = this;

    return true;

}


// default constructor
Quad::Quad() {}
// destructor
Quad::~Quad() {}

Quad::Quad(point3 p1, point3 p2, point3 p3, Material* material) {
    // middle, right, top
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->material = material;
}

/*
 Quad intersection
  - solving equation for a point on ray which intersects plane and stores info in intersect
*/
bool Quad::get_intersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {

    // bottom left front corner: position
    // bottom plane: x = btc + u(0, 0, -height) + v(width, 0, 0)

    // getting the 2 vectors defining the plane
    // a = p1, b = p2, c= p3
    glm::dvec3 p = p2 - p1; // p = b-a
    glm::dvec3 q = p3 - p1; // p = c-a
    glm::dvec3 tmp1 = glm::cross(ray.direction(), q);
    double dot1 = glm::dot(tmp1, p);

    if (glm::abs(dot1) < DBL_EPSILON) {
        return false;
    }

    double f = 1 / dot1;
    
    glm::dvec3 s = ray.origin() - p1;
    double u = f * glm::dot(s, tmp1);
    if ((u < 0.0) || (u > 1.0)) {
        return false;
    }

    glm::dvec3 tmp2 = glm::cross(s, p);
    double v = f * glm::dot(ray.direction(), tmp2);
 
    if ((v < 0.0) || (v > 1.0)) {
        return false;
    }

    double t = f * glm::dot(q, tmp2);
    // why not absolute? 
    if (t < DBL_EPSILON) {
        return false;
    }

    // store intersection info
    intersect.t = t;
    // point of intersection in global coordinate
    intersect.point = ray.at(intersect.t);
    // unnormalized normal
    glm::dvec3 normal = glm::cross(q, p);
    intersect.set_face_normal(ray, glm::normalize(normal));
    intersect.material = material;
    intersect.object = this;

    return true;
}


