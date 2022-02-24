#include "material.h"

// pragma is same as ifndef and define to prevent many imports

//Material::Material(glm::dvec3 diffuse, glm::dvec3 specular, double shininess) {
//    this->diffuse = diffuse;
//    this->specular = specular;
//    this->shininess = shininess;
//};

//Material::Material(glm::dvec3 diffuse, glm::dvec3 specular, double shininess) :
//    diffuse(diffuse), specular(specular), shininess(shininess) {};

// INIT MATERIAL 
Lambertian::Lambertian(const color& albedo) {
    this->diffuse = albedo;
    this->specular = glm::dvec3(0.0);
    this->shininess = 0.0;
}

// TODO MAKE SHINY
Metal::Metal(const color& albedo) {
    this->diffuse = albedo;
    this->specular = glm::dvec3(0.0);
    this->shininess = 0.0;
}

// pass in elements of intersection? 
    // material doesn't need to know about the object
bool Lambertian::scatter(
    const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
) const {
    // use the other?? regular diffuse? 
    glm::dvec3 scatter_direction = intersect.normal + random_unit_vector();

    if (near_zero(scatter_direction)) {
        // If the random unit vector we generate is exactly opposite the normal vector, the two will sum to zero, which will result in a zero scatter direction vector.
        scatter_direction = intersect.normal;
    }

    scattered = Ray(intersect.point, scatter_direction);
    attenuation = diffuse; // albedo == diffuse for lambertian
    return true;
}

bool Metal::scatter(
    const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
) const {
    glm::dvec3 reflected = glm::reflect(glm::normalize(r_in.direction()), intersect.normal);
    scattered = Ray(intersect.point, reflected);
    attenuation = diffuse;
    return (glm::dot(scattered.direction(), intersect.normal) > 0);
}