#include "ray.h"

struct Camera {

    Vector direction, position, right, up;

    Camera(const Vector &position, const Vector &lookAt);
    Ray trace(const unsigned x, const unsigned y) const;

};