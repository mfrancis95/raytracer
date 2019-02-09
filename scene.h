#include <vector>
#include "camera.h"
#include "illumination.h"
#include "primitive.h"

typedef struct {
    Camera camera;
    std::vector<Illumination *> illuminations;
    std::vector<Light> lights;
    std::vector<Material> materials;
    std::vector<Primitive *> primitives;
} Scene;