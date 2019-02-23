#include "vector.h"

typedef struct {
    Vector colour;
    double diffuse;
    double reflectivity;
    double shininess;
    double specular;
} Material;