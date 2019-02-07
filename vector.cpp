#include <cmath>
#include "vector.h"

Vector::Vector(double x, double y, double z) : x{x}, y{y}, z{z} {}

double Vector::length() {
    return std::sqrt(x * x + y * y + z * z);
}

Vector &Vector::multiply(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector &Vector::normalise() {
    double len = length();
    return len ? multiply(1 / len) : *this;
}

Vector &Vector::operator+=(const Vector &vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}