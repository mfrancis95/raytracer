#include <vector>
#include "camera.h"
#include "material.h"
#include "primitive.h"

typedef struct {
    Camera camera;
    std::vector<Material> materials;
    std::vector<Primitive *> primitives;
} Scene;