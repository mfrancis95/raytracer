#include "illumination.h"

struct Phong : Illumination {

    Vector illuminate(
        const Intersection &intersection, const Light &light,
        const Material &material
    ) const;

};