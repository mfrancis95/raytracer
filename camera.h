#include "ray.h"

struct Camera {

    Vector direction, position, right, up;

    Camera();
    Camera(const Vector &position, const Vector &lookAt);
    Ray trace(const double x, const double y) const;

};