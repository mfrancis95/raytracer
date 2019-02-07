#include "primitive.h"

struct Sphere : Primitive {

    double radiusSquared;
    Vector position;

    Sphere(Vector &position, double radius);
    Intersection intersect(const Ray &ray) const;

};