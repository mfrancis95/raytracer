#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

struct Vector {

    double x, y, z;

    Vector() = default;
    Vector(double x, double y, double z);
    double dot(const Vector &vector) const;
    double length() const;
    double lengthSquared() const;
    Vector &normalise();
    Vector operator*(const double scalar) const;
    Vector operator*(const Vector &vector) const;
    Vector operator-(const Vector &vector) const;
    Vector operator+(const Vector &vector) const;
    Vector &operator+=(const Vector &vector);

};

std::ostream& operator<<(std::ostream &os, const Vector &vector);

#endif