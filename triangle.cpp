#include <algorithm>
#include <cmath>
#include "triangle.h"

Triangle::Triangle(
    const Vector &vertex1, const Vector &vertex2, const Vector &vertex3
) : vertex1{vertex1}, vertex2{vertex2}, vertex3{vertex3} {}

Intersection Triangle::intersect(const Ray &ray) const {
    auto v0v1 = vertex2 - vertex1;
    auto v0v2 = vertex3 - vertex1;
    auto pVec = ray.direction * v0v2;
    auto det = v0v1.dot(pVec); 
    if (std::abs(det) < 1e-8) {
        return NO_INTERSECTION;
    }
    auto invDet = 1 / det;
    auto tVec = ray.origin - vertex1;
    auto u = tVec.dot(pVec) * invDet;
    if (u < 0 || u > 1) {
        return NO_INTERSECTION;
    }
    auto qVec = tVec * v0v1;
    auto v = ray.direction.dot(qVec) * invDet;
    if (v < 0 || u + v > 1) {
        return NO_INTERSECTION;
    }
    auto t = v0v2.dot(qVec) * invDet;
    if (t < 0) {
        return NO_INTERSECTION;
    }
    return {t, (v0v1 * v0v2).normalise(), ray.direction * t + ray.origin};
}