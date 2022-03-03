#include "camera.h"

// static camera
Camera::Camera(double image_width, double image_height) {
    resolution_x = image_width;
    resolution_y = image_height;
    
    aspect_ratio = resolution_x / resolution_y; 
    // camera's direction
    this->dir = glm::normalize(look_at - position);
    // camera's/image plane's right (horizontal) vector
    this->u = glm::normalize(glm::cross(this->dir, this->up));
    // camera's/image plane's up (vertical) vector
    this->v = glm::normalize(glm::cross(u, dir));

    point3 img_plane_center = position + dir * focal_length;
    double img_plane_height = 2.0 * focal_length * tan((fovy / 2.0) * pi / 180);
    double img_plane_width = img_plane_height * aspect_ratio;
    step_x = img_plane_width / resolution_x;
    step_y = img_plane_height / resolution_y;

    // will start drawing from lower left corner
    lower_left_corner = img_plane_center - (u * (img_plane_width / 2)) - (v * (img_plane_height / 2));
}

Ray Camera::get_ray(int i, int j) const {
    // convert pixel i, j to image plane's u, v coordinates
    point3 pixel_pos = lower_left_corner + ((u * (double)i) * step_x) + ((v * (double)j) * step_y);
    glm::dvec3 ray_direction = glm::normalize(pixel_pos - position);

    return Ray(this->position, ray_direction);
}