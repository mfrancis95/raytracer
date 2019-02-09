#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include "intersection.h"
#include "light.h"
#include "material.h"

struct Illumination {

    virtual Vector illuminate(
        const Intersection &intersection, const Light &light,
        const Material &material
    ) const = 0;

};

#endif