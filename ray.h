#ifndef RAY_H
#define RAY_H

#include "vector.h"

typedef struct {
    Vector direction;
    Vector origin;
} Ray;

#endif