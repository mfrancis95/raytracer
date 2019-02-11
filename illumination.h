#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include "intersection.h"
#include "light.h"
#include "material.h"
#include "ray.h"

struct Illumination {

    virtual Vector illuminate(
        const Intersection &intersection, const Light &light,
        const Material &material, const Ray &ray
    ) const = 0;

};

#endif