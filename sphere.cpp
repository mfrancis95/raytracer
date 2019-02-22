#include <algorithm>
#include <cmath>
#include "sphere.h"

Sphere::Sphere(const Vector &position, const double radius) {
    this->position = position;
    radiusSquared = radius * radius;
}

Intersection Sphere::intersect(const Ray &ray) const {
    auto difference = ray.origin - position;
    auto b = 2 * difference.dot(ray.direction);
    auto disc = b * b - 4 * (difference.lengthSquared() - radiusSquared);
    if (disc < 0) {
        return NO_INTERSECTION;
    }
    auto discSqrt = std::sqrt(disc);
    auto t0 = (-b - discSqrt) / 2;
    auto t1 = (-b + discSqrt) / 2;
    if (t0 > t1) {
        std::swap(t0, t1);
    }
    if (t1 < 0) {
        return NO_INTERSECTION;
    }
    if (t0 < 0) {
        auto point = ray.direction * t1 + ray.origin;
        return {t1, (point - position).normalise(), point};
    }
    auto point = ray.direction * t0 + ray.origin;
    return {t0, (point - position).normalise(), point};
}

void Sphere::serialise(void *buffer) const {
    *static_cast<int *>(buffer) = 0;
    position.serialise(buffer + 16);
    *static_cast<float *>(buffer + 28) = radiusSquared;
}