#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class Camera {
public:
   
    Camera(double image_width, double image_height);

    Ray get_ray(int i, int j) const;
    // looking into the negative z
    point3 position = point3(0.0, 0.0, 20.0); // eye


private:
        point3 lower_left_corner;

    double fovy = 90;
    double focal_length = 1 / tan(fovy / 2);
    double resolution_x = 800;
    double resolution_y = 800;
    double aspect_ratio = resolution_x / resolution_y ;
    point3 look_at = point3(0.0, 0.0, 0.0); // looking at origin
    glm::dvec3 up = glm::dvec3(0.0, 1.0, 0.0); // positive y
    glm::dvec3 dir; // to look at 
    glm::dvec3 v; // camera up
    glm::dvec3 u; // camera right

    double step_x = 0;
    double step_y = 0;
    
};

#endif