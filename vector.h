#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

struct Vector {

    double x, y, z;

    Vector();
    Vector(unsigned colour);
    Vector(double x, double y, double z);
    double dot(const Vector &vector) const;
    double length() const;
    double lengthSquared() const;
    Vector &normalise();
    operator unsigned() const;
    Vector operator*(const double scalar) const;
    Vector operator*(const Vector &vector) const;
    Vector operator-(const Vector &vector) const;
    Vector operator+(const Vector &vector) const;
    Vector &operator+=(const Vector &vector);
    void serialise(char *buffer) const;

};

std::ostream& operator<<(std::ostream &os, const Vector &vector);

#endif