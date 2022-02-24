#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class Camera {
public:
    Camera() {
        auto aspect_ratio = 16.0 / 9.0; // auto for no conversion
        // viewport two units in height
        double viewport_height = 2.0;
        double viewport_width = aspect_ratio * viewport_height;
        // distance between the projection plane and the projection point (camera) to be one unit ( depth in z)
        double focal_length = 1.0;

        position = point3(0.0, 0.0, 0.0); //camera/eye/origin (center of image)
        horizontal = glm::dvec3(viewport_width, 0.0, 0.0);
        vertical = glm::dvec3(0.0, viewport_height, 0.0);
        // lower_left_corner (of the image projection plane): center - half the width - half the height // we are looking into the negative z direction 
        // positive x to the right, positive y up
        lower_left_corner = position - horizontal / 2.0 - vertical / 2.0 - glm::dvec3(0.0, 0.0, focal_length);
    }

    Ray get_ray(double u, double v) const {

        // ray direction = end - origin = pixel position on projection plane - camera_pos (ray pointing into the scene)
        // because we are looking into the NEGATIVE Z!! so the projection plane is forward in the z direction 
        // camera ray
        return Ray(this->position, lower_left_corner + u * horizontal + v * vertical - position);
    }


private:
    point3 position;
    point3 lower_left_corner;
    glm::dvec3 horizontal;
    glm::dvec3 vertical;
};

#endif