#pragma once
#ifndef UTILITY_H
#define UTILITY_H

// pragma is same as ifndef and define to prevent many imports (for old and new cpp)

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <cstdlib>
#include <iostream>

// Type aliases for vec3
using point3 = glm::dvec3; // 3D point
using color = glm::dvec3; // RGB color

// Constants
const double pi = 3.1415926535897932385;

// Utility Functions
//void print_vec3(const char* name, glm::vec3 vec3) {
//    std::cout << name << ": " << vec3.x << " " << vec3.y << " " << vec3.z << std::endl;
//}

#include "ray.h"

#endif