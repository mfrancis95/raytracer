#include "vector.h"

Vector::Vector() {}

Vector::Vector(double x, double y, double z) : x{x}, y{y}, z{z} {}

Vector &Vector::operator+=(const Vector &vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}