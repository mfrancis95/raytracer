#include <limits>
#include "vector.h"

typedef struct {
    double distance;
    Vector point;
} Intersection;

const Intersection NO_INTERSECTION = {std::numeric_limits<double>::infinity()};