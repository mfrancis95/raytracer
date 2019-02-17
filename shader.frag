#version 440

layout(origin_upper_left) in vec4 gl_FragCoord;

struct Camera {
    vec3 direction;
    vec3 position;
    vec3 right;
    vec3 up;
};

uniform Camera camera;

struct Intersection {
    float distance;
    vec3 normal;
    vec3 point;
};

struct Light {
    vec3 colour;
    vec3 position;
};

struct Material {
    vec3 colour;
    float shininess;
};

struct Ray {
    vec3 direction;
    vec3 origin;
};

struct Sphere {
    vec3 position;
    float radiusSquared;
};

Ray castRay(float x, float y) {
    return Ray(
        normalize(camera.right * x + camera.up * y + camera.direction),
        camera.position
    );
}

bool sphereIntersect(out Intersection intersection, Sphere sphere, Ray ray) {
    vec3 difference = ray.origin - sphere.position;
    float b = 2 * dot(difference, ray.direction);
    float disc = b * b - 4 * (dot(difference, difference) - sphere.radiusSquared);
    if (disc < 0) {
        return false;
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
        return false;
    }
    if (t0 < 0) {
        intersection.distance = t1;
        intersection.point = ray.direction * t1 + ray.origin;
    }
    else {
        intersection.distance = t0;
        intersection.point = ray.direction * t0 + ray.origin;
    }
    intersection.normal = normalize(intersection.point - sphere.position);
    return true;
}

vec3 phongIlluminate(
    Intersection intersection, Light light, Material material, Ray ray
) {
    vec3 L = normalize(light.position - intersection.point);
    float LdotN = max(0, dot(L, intersection.normal));
    vec3 colour = material.colour * LdotN;
    if (LdotN > 0) {
        vec3 R = normalize(intersection.normal * 2.0 * LdotN - L);
        vec3 V = normalize(ray.origin - intersection.point);
        colour += light.colour * pow(max(0.0, dot(R, V)), material.shininess);
    }
    return colour;
}

out vec3 colour;

void main() {
    Intersection intersection = {0, {0, 0, 0}, {0, 0, 0}};
    Light light = {{1, 1, 1}, {4, 2, 0}};
    Material material = {{1, 0, 0}, 10};
    Sphere sphere = {{0, -1, -3}, 0.25};
    float aspectRatio = 1280.0 / 960;
    Ray ray = castRay(gl_FragCoord.x / 1280 * aspectRatio - 0.5, gl_FragCoord.y / 960 - 0.5);
    if (sphereIntersect(intersection, sphere, ray)) {
        colour = phongIlluminate(intersection, light, material, ray);
    }
    else {
        colour = vec3(0, 0, 0);
    }
}