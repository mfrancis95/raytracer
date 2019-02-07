#include <algorithm>
#include <cmath>
#include <limits>
#include "sphere.h"

Sphere::Sphere(Vector &position, double radius) {
    this->position = position;
    radiusSquared = radius * radius;
}

Intersection Sphere::intersect(const Ray &ray) const {
    Vector difference = ray.origin - position;
    double b = 2 * difference.dot(ray.direction);
    double c = difference.lengthSquared() - radiusSquared;
    double disc = b * b - 4 * (difference.lengthSquared() - radiusSquared);
    if (disc < 0) {
        return {std::numeric_limits<double>::infinity()};
    }
    double discSqrt = std::sqrt(disc);
    double t0 = (-b - discSqrt) / 2;
    double t1 = (-b + discSqrt) / 2;
    if (t0 > t1) {
        std::swap(t0, t1);
    }
    if (t1 < 0) {
        return {std::numeric_limits<double>::infinity()};
    }
    if (t0 < 0) {
        return {t1, ray.direction * t1 + ray.origin};
    }
    return {t0, ray.direction * t0 + ray.origin};
}