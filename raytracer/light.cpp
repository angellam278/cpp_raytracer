#include "light.h"


// default constructor
Light::Light() {}
// destructor
Light::~Light() {}

Light::Light(point3 position, color diffuse, color specular) {
    this->position = position;
    this->diffuse = diffuse;
    this->specular = specular;
}
