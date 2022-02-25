#include "material.h"

// pragma is same as ifndef and define to prevent many imports


Material::Material(color diffuse, color specular, float shininess) :
    diffuse(diffuse), specular(specular), shininess(shininess) {};

bool Material::scatter(
    const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
) const {
    // use the other?? regular diffuse? NORMALIZE???
    glm::dvec3 scatter_direction = intersect.normal + random_unit_vector();
    if (near_zero(scatter_direction)) {
        // If the random unit vector we generate is exactly opposite the normal vector, the two will sum to zero, which will result in a zero scatter direction vector.
        scatter_direction = intersect.normal;
    }

    scattered = Ray(intersect.point, glm::normalize(scatter_direction));
    attenuation = diffuse; // albedo == diffuse for lambertian
    return true;
}
