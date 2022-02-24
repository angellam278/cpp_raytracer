#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "object.h"

class Material {
public:
    color diffuse;
    glm::dvec3 specular;
    double shininess;

    //Material(glm::dvec3 diffuse, glm::dvec3 specular, double shininess);
    virtual bool scatter(
        const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
    ) const = 0;
};


class Lambertian : public Material {
public:
    Lambertian(const color& albedo);

    virtual bool scatter(
        const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
    ) const override;
};


class Metal : public Material {
public:
    Metal(const color& albedo);

    virtual bool scatter(
        const Ray& r_in, const intersection& intersect, color& attenuation, Ray& scattered
    ) const override;
};

#endif