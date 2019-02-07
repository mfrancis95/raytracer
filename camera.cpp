#include "camera.h"

Camera::Camera(const Vector &position, const Vector &lookAt) {
    this->position = position;
    direction = (lookAt - position).normalise();
    right = (direction * Vector{0, 1, 0}).normalise();
    up = (direction * right).normalise();
}

Ray Camera::trace(const unsigned x, const unsigned y) const {
    return {position, (right * x + up * y + direction).normalise()};
}