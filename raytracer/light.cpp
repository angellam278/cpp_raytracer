#include "light.h"

Light::Light(point3 position, color diffuse, color specular) {
    this->position = position;
    this->diffuse = diffuse;
    this->specular = specular;
}