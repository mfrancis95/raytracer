#include "primitive.h"

struct Triangle : Primitive {

    Vector vertex1, vertex2, vertex3;

    Triangle(const Vector &vertex1, const Vector &vertex2, const Vector &vertex3);
    Intersection intersect(const Ray &ray) const;
    void serialise(void *buffer) const;

};