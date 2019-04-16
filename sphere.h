#include "primitive.h"

struct Sphere : Primitive {

    Vector position;
    double radiusSquared;

    Sphere(const Vector &position, const double radius);
    Intersection intersect(const Ray &ray) const;
    void serialise(char *buffer) const;
    void translate(const Vector &vector);

};