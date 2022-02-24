#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <random>
#include <cstdlib>
#include <iostream>

// forward ref
class Ray;

// Type aliases for vec3
using point3 = glm::dvec3;   // 3D point
using color = glm::dvec3;    // RGB color

const double epsilon = std::numeric_limits<double>::epsilon();

//inline double random_double() {
//    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//    static std::mt19937 generator;
//    return distribution(generator);
//}
double random_double();

glm::vec3 random(double min, double max);

glm::dvec3 random_in_unit_sphere();

glm::dvec3 random_unit_vector();

glm::dvec3 random_in_hemisphere(const glm::dvec3& normal);

bool near_zero(const glm::dvec3& vec);


void print_vec3(const char* name, glm::vec3 vec3);

#endif
