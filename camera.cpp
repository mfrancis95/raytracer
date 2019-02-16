#include "camera.h"

Camera::Camera() : Camera{{0, 0, 0}, {0, 0, -1}} {}

Camera::Camera(const Vector &position, const Vector &lookAt) {
    this->position = position;
    direction = (lookAt - position).normalise();
    right = (direction * Vector{0, 1, 0}).normalise();
    up = (direction * right).normalise();
}

Ray Camera::castRay(const double x, const double y) const {
    return {(right * x + up * y + direction).normalise(), position};
}