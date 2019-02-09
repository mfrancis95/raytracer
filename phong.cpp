#include <algorithm>
#include "phong.h"

Vector Phong::illuminate(
    const Intersection &intersection, const Light &light,
    const Material &material
) {
    return material.colour * std::max(0.0, (light.position - intersection.point).normalise().dot(intersection.normal));
}