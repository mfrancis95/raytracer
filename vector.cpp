#include <cmath>
#include "vector.h"

Vector::Vector(double x, double y, double z) : x{x}, y{y}, z{z} {}

double Vector::dot(const Vector &vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}

double Vector::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector &Vector::normalise() {
    double len = length();
    if (len) {
        x /= len;
        y /= len;
        y /= len;
    }
    return *this;
}

Vector Vector::operator*(const double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

Vector Vector::operator*(const Vector &vector) const {
    return {y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x};
}

Vector Vector::operator-(const Vector &vector) const {
    return {x - vector.x, y - vector.y, z - vector.z};
}

Vector Vector::operator+(const Vector &vector) const {
    return {x + vector.x, y + vector.y, z + vector.z};
}

Vector &Vector::operator+=(const Vector &vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}