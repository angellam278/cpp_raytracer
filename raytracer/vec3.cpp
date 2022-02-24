#include "vec3.h"

// TODO RENAME TO MATH UTILITY

double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

glm::vec3 random(double min, double max) {
    return glm::vec3(random_double(), random_double(), random_double());
}

glm::dvec3 random_in_unit_sphere() {
    while (true) {
        glm::dvec3 rand_v = random(-1.0, 1.0);
        if (glm::length2(rand_v) >= 1) continue;
        return rand_v;
    }
}

glm::dvec3 random_unit_vector() {
    return glm::normalize(random_in_unit_sphere());
}

glm::dvec3 random_in_hemisphere(const glm::dvec3& normal) {
    glm::dvec3 in_unit_sphere = random_in_unit_sphere();
    if (glm::dot(in_unit_sphere, normal) > 0.0)
        // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

bool near_zero(const glm::dvec3& vec) {
    // Return true if the vector is close to zero in all dimensions.
    return (glm::abs(vec.x) < epsilon) && (glm::abs(vec.y) < epsilon) && (glm::abs(vec.z) < epsilon);
}

void print_vec3(const char* name, glm::vec3 vec3) {
    std::cout << name << ": " << vec3.x << " " << vec3.y << " " << vec3.z << std::endl;
}
