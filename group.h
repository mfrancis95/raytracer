#include <vector>
#include "primitive.h"

struct Group : Primitive {

    Group(const std::vector<Primitive *> &primitives);
    Intersection intersect(const Ray &ray) const;

private:

    std::vector<Primitive *> primitives;
    
};