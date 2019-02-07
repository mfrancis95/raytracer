#include "intersection.h"
#include "ray.h"

struct Primitive {

    virtual Intersection intersect(const Ray &ray) const = 0;

};