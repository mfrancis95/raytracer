#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "intersection.h"
#include "ray.h"

struct Primitive {

    virtual Intersection intersect(const Ray &ray) const = 0;
    virtual void serialise(void *buffer) const = 0;

};

#endif