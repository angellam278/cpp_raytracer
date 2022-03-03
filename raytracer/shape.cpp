
#include "shape.h"

Sphere::Sphere() {}

Sphere::Sphere(point3 center, double radius, Material* material) {
    this->position = center;
    this->radius = radius;
    this->material = material;
}

Sphere::~Sphere() {}

Quad::Quad() {}
Quad::~Quad() {}

Quad::Quad(point3 p1, point3 p2, point3 p3, Material* material) {
    // middle, right, top
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->material = material;
}

//
//bool Quad::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {
//
//    // bottom left front corner: position
//    // bottom plane: x = btc + u(0, 0, -height) + v(width, 0, 0)
//
//    glm::dvec3 v1 = p2 - p1;
//    glm::dvec3 v2 = p3 - p1;
//    //print_vec3("v1", v1);
//    //print_vec3("v2", v2);
//
//    double width = glm::length(v1);
//    double height = glm::length(v2);
//    //std::cout << width << std::endl;
//    //std::cout << height << std::endl;
//    glm::dvec3 normal = glm::cross(v1, v2);
//    //print_vec3("normal", normal);
//    /*print_vec3("end1", end1);
//    print_vec3("end2", end2);
//    print_vec3("v1", v1);
//    print_vec3("v2", v2);
//    */
//    double dd = glm::dot(normal, p1);
//    //std::cout << dd << std::endl;
//    double denom = glm::dot(normal, ray.direction());
//    //std::cout << denom << std::endl;
//    //  make overloaded near_zero: parallel 
//    if (glm::abs(denom) < epsilon) {
//        //std::cout << "denom near 0" << std::endl;
//        return false;
//    }
//    double t = -(glm::dot(normal, (ray.origin() - p1))) / denom;
//    point3 poi = ray.at(t);
//    double u = glm::dot((poi - p1), v1);
//    double v = glm::dot((poi - p1), v2);
//
//    if ((t < infinity) && (t > 0.0) && (abs(u) <= width) && (abs(v) <= height)) {
//        intersect.t = t;
//        // point of intersection in global coordinate
//        intersect.point = ray.at(intersect.t);
//        // unnormalized normal
//        intersect.set_face_normal(ray, normal);
//        // return point of intersection ? or object just updates its normal? 
//        //intersect.material_ptr = material_ptr;
//        intersect.material = material;
//        intersect.object = this;
//        return true;
//    }
//     
//    return false;
//}



bool Quad::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {

    // bottom left front corner: position
    // bottom plane: x = btc + u(0, 0, -height) + v(width, 0, 0)

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
    intersect.t = t;
    // point of intersection in global coordinate
    intersect.point = ray.at(intersect.t);
    // unnormalized normal
    glm::dvec3 normal = glm::cross( q, p);
    // NEED TO NORMALIZE!!
    intersect.set_face_normal(ray, glm::normalize(normal));

    // return point of intersection ? or object just updates its normal? 
    intersect.material = material;
    intersect.object = this;

    return true;
}



/*
    Sphere intersection
        - solving equation for a point on ray which is at distance r from center
*/
bool Sphere::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {

    // (p-c).(p-c) = r^2 where p = point on sphere, c = center of sphere and r is the radius
    // p = ray_origin + t(ray_direction), we are trying to find the t in which the ray intersects the sphere
    // --> ((ray_origin + t(ray_direction)) - c).((ray_origin + t(ray_direction)) - c) = r^2
    // --> ((ray_origin - c) + t(ray_direction)).((ray_origin - c) + t(ray_direction)) = r^2
    // -- > (origin_center + t(ray_direction)).(origin_center + t(ray_direction)) = r^2
    // -- > (ray_direction.ray_direction)t^2 + 2(origin_center.ray_direction)t + (origin_center.origin_center - r^2) = 0
    // -- > at^2 + 2bt + c = 0

    // bring ray to local coordinate
    //Ray back_ray = glm::inverse(transform_matrix) * ray;

    //ray_direction.ray_direction
    //double a = 1.0; 
    //glm::length2(back_ray.direction());
    glm::dvec3 oc = ray.origin() - position;
    double a = glm::length2(ray.direction());
    double b = 2 * glm::dot(oc, ray.direction());
    double c = glm::length2(oc) - (radius * radius);

    double discriminant = (b * b) - 4 * a * c;
    //std::cout << "discriminant: " << discriminant << std::endl;
    if (discriminant < 0) { return false; }
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range. (already checking epsilon here)
    double root = (-b - sqrtd) / 2 * a;
    if (root < t_min || t_max < root) {
        root = (-b + sqrtd) / 2 * a;
        if (root < t_min || t_max < root)
            return false;
    }

    //double roota = (-b - sqrtd) / 2 * a;
    //double rootb = (-b + sqrtd) / 2 * a;
    //if (roota < 0.0 && rootb < 0.0) {
    //    // no intersection 
    //    return false;
    //}
    //// root a should be less than root b
    //// we want the earlier intersection
    //double t = roota;
    //if (roota > 0.0) {
    //    if (roota < t_min || roota > t_max) {
    //        return false;
    //    }
    //    t = roota;
    //}
    //else {
    //    // root a is behind so look at root b ( already check case both is under 0.0)
    //    // so if a is < 0.0 then b must be > 0.0
    //    if (rootb < t_min || rootb > t_max) {
    //        // too close to zero
    //        return false;
    //    }
    //    t = rootb;
    //}

    // record intersection info: DO WE NEED?
    intersect.t = root;
    // point of intersection in global coordinate
    intersect.point = ray.at(intersect.t);
    // normalized normal
    glm::dvec3 outward_normal = (intersect.point - position) / radius;
    intersect.set_face_normal(ray, outward_normal);
    // return point of intersection ? or object just updates its normal? 
    //intersect.material_ptr = material_ptr;
    intersect.material = material;
    intersect.object = this;

    return true;

}

glm::dvec3 Sphere::getNormal(const point3 point) {
    // normalized normal (center = position)
    glm::dvec3 normal = point - position / radius;
    return normal;
}
