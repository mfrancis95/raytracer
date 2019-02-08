#include "primitive.h"

struct Sphere : Primitive {

    double radiusSquared;
    Vector position;

    Sphere(const Vector &position, const double radius);
    Intersection intersect(const Ray &ray) const;

};