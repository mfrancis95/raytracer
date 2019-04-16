#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "intersection.h"
#include "ray.h"

struct Primitive {

    virtual Intersection intersect(const Ray &ray) const = 0;
    virtual void serialise(char *buffer) const = 0;
    virtual void translate(const Vector &translation) = 0;

};

#endif