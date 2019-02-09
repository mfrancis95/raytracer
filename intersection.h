#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <limits>
#include "vector.h"

typedef struct {
    double distance;
    Vector normal;
    Vector point;
} Intersection;

const Intersection NO_INTERSECTION = {std::numeric_limits<double>::infinity()};

#endif