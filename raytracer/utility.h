#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <iostream>

// TODO RENAME TO COMMON 

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
// inline: (Faster) Compiler replaces the definition of inline functions at compile time instead of referring function definition at runtime.
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

//constexpr is a feature added in C++ 11. The main idea is performance improvement of programs by doing computations at compile time rather than run time. 
// Define direction flag values.
constexpr bool FWDTFORM = true;
constexpr bool BCKTFORM = false;
//
//inline double random_double(double min, double max) {
//    // Returns a random real in [min,max).
//    return min + (max - min) * random_double();
//}

#include "ray.h"
#include "vec3.h"


#endif