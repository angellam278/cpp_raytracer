
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
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->material = material;
}


bool Quad::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {

    // bottom left front corner: position
    // bottom plane: x = btc + u(0, 0, -height) + v(width, 0, 0)

    glm::dvec3 v1 = p2 - p1;
    glm::dvec3 v2 = p3 - p1;
    //print_vec3("v1", v1);
    //print_vec3("v2", v2);

    double width = glm::l2Norm(p2, p1);
    double height = glm::l2Norm(p3, p1);
    //std::cout << width << std::endl;
    //std::cout << height << std::endl;
    glm::dvec3 normal = glm::cross(v2, v1);
    /*print_vec3("end1", end1);
    print_vec3("end2", end2);
    print_vec3("v1", v1);
    print_vec3("v2", v2);
    print_vec3("normal", normal);*/
    double dd = glm::dot(normal, p1);
    //std::cout << dd << std::endl;
    double denom = glm::dot(normal, ray.direction());
    //std::cout << denom << std::endl;
    //  make overloaded near_zero: parallel 
    if (glm::abs(denom) < epsilon) {
        //std::cout << "denom near 0" << std::endl;
        return false;
    }
    double t = -(glm::dot(normal, (ray.origin() - p1))) / denom;
    point3 poi = ray.at(t);
    double u = glm::dot((poi - p1), v1);
    double v = glm::dot((poi - p1), v2);

    if ((t < infinity) && (t > 0.0) && (abs(u) <= width) && (abs(v) <= height)) {
        intersect.t = t;
        // point of intersection in global coordinate
        intersect.point = ray.at(intersect.t);
        // unnormalized normal
        intersect.set_face_normal(ray, normal);
        // return point of intersection ? or object just updates its normal? 
        //intersect.material_ptr = material_ptr;
        intersect.material = material;
        intersect.object = this;
        return true;
    }
     
    return false;
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
    auto half_b = glm::dot(oc, ray.direction());
    auto c = glm::length2(oc) - (radius * radius);

    auto discriminant = half_b * half_b - a * c;
    //std::cout << "discriminant: " << discriminant << std::endl;
    if (discriminant < 0) { return false; }
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

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
