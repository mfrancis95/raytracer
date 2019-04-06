layout(origin_upper_left) in vec4 gl_FragCoord;

out vec3 colour;

void main() {
    colour = vec3(0, 0, 0);
    Intersection closestIntersection = {3.402823466e+38, {0, 0, 0}, {0, 0, 0}};
    Intersection intersection = closestIntersection;
    int primitive = -1;
    float aspectRatio = 1280.0 / 960;
    Ray ray = castRay(
        (gl_FragCoord.x / 1280 - 0.5) * aspectRatio, gl_FragCoord.y / 960 - 0.5
    );
    for (int i = 0; i < numPrimitives; i++) {
        if (intersect(primitives[i], ray, intersection) &&
            intersection.distance < closestIntersection.distance) {
            closestIntersection = intersection;
            primitive = i;
        }
    }
    if (primitive == -1) {
        return;
    }
    colour = ambient;
    for (int i = 0; i < numLights; i++) {
        Ray lightRay = {
            normalize(lights[i].position - closestIntersection.point),
            closestIntersection.point
        };
        bool intersectsPrimitive = false;
        for (int j = 0; j < numPrimitives; j++) {
            if (j != primitive &&
                intersect(primitives[j], lightRay, intersection)) {
                intersectsPrimitive = true;
                break;
            }
        }
        if (!intersectsPrimitive) {
            colour += phongIlluminate(
                closestIntersection, lights[i], materials[primitive], ray
            );
        }
    }
}