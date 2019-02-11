#include <algorithm>
#include <cmath>
#include "phong.h"

Vector Phong::illuminate(
    const Intersection &intersection, const Light &light,
    const Material &material, const Ray &ray
) const {
    auto L = (light.position - intersection.point).normalise();
    auto LdotN = std::max(0.0, L.dot(intersection.normal));
    Vector colour = material.colour * LdotN;
    if (LdotN) {
        auto R = (intersection.normal * 2.0 * LdotN - L).normalise();
        auto V = (ray.origin - intersection.point).normalise();
        colour += light.colour * std::pow(std::max(0.0, R.dot(V)), material.shininess);
    }
    return colour;
}