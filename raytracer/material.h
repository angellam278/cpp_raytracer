#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "object.h"


class Material {

    public:
        Material();
        ~Material();
        Material(color diffuse, color specular, float shininess);

        color diffuse;
        color specular;
        float shininess;
        float refraction_index = 0.5; // cut attenuation in half

        bool scatter(
            const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
        ) const;

};


#endif