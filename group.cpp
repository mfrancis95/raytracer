#include <cmath>
#include "group.h"

Group::Group(const std::vector<Primitive *> &primitives) :
    primitives{primitives} {}

Intersection Group::intersect(const Ray &ray) const {
    for (auto &primitive : primitives) {
        Intersection intersection = primitive->intersect(ray);
        if (!std::isinf(intersection.distance)) {
            return intersection;
        }
    }
    return NO_INTERSECTION;
}