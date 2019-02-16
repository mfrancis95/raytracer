#version 440

struct Camera {
    vec3 direction;
    vec3 position;
    vec3 right;
    vec3 up;
};

struct Ray {
    vec3 direction;
    vec3 origin;
};

struct Sphere {
    vec3 position;
    float radiusSquared;
};

Ray castRay(Camera camera, float x, float y) {
    return Ray(normalize(camera.right * x + camera.up * y + camera.direction), camera.position);
}

int sphereIntersects(Sphere sphere, Ray ray) {
    vec3 difference = ray.origin - sphere.position;
    float b = 2 * dot(difference, ray.direction);
    float disc = b * b - 4 * (dot(difference, difference) - sphere.radiusSquared);
    if (disc < 0) {
        return 0;
    }
    float discSqrt = sqrt(disc);
    float t0 = (-b - discSqrt) / 2;
    float t1 = (-b + discSqrt) / 2;
    if (t0 > t1) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    if (t1 < 0) {
        return 0;
    }
    return 1;
}

out vec3 colour;

void main() {
    Camera camera = Camera(
        vec3(0, 0, -1), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 1, 0)
    );
    Sphere sphere = Sphere(vec3(0, 0, -5), 1);
    float aspectRatio = 1280.0 / 960;
    Ray ray = castRay(camera, gl_FragCoord.x / 1280 * aspectRatio - 0.5, gl_FragCoord.y / 960 - 0.5);
    if (sphereIntersects(sphere, ray) == 1) {
        colour = vec3(1, 0, 0);
    }
    else {
        colour = vec3(0, 0, 0);
    }
}