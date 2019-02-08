#include <forward_list>
#include "camera.h"
#include "primitive.h"

typedef struct {
    Camera camera;
    std::forward_list<Primitive *> primitives;
} Scene;