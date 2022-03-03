#include "material.h"


// default constructor
Material::Material() {}
// destructor
Material::~Material() {}

Material::Material(color diffuse, color specular, float shininess) :
    diffuse(diffuse), specular(specular), shininess(shininess) {};