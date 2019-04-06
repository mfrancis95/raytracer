#version 440

struct Camera {
    vec3 direction;
    vec3 position;
    vec3 right;
    vec3 up;
};

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
    float diffuse;
    float shininess;
    float specular;
};

struct Ray {
    vec3 direction;
    vec3 origin;
};

struct Sphere {
    vec3 position;
    float radiusSquared;
};

struct Triangle {
    vec3 vertex1;
    vec3 vertex2;
    vec3 vertex3;
};

struct Primitive {
    int type;
    vec3 spherePosition;
    float sphereRadiusSquared;
    vec3 triangleVertex1;
    vec3 triangleVertex2;
    vec3 triangleVertex3;
};

layout(location = 0) uniform vec3 ambient;
layout(location = 1) uniform Camera camera;
layout(location = 5) uniform int numLights;
layout(location = 6) uniform int numPrimitives;

layout(binding = 0, std430) buffer LightStorage {
    Light lights[];
};

layout(binding = 1, std430) buffer MaterialStorage {
    Material materials[];
};

layout(binding = 2, std430) buffer PrimitiveStorage {
    Primitive primitives[];
};

Ray castRay(float x, float y) {
    return Ray(
        normalize(camera.right * x + camera.up * y + camera.direction),
        camera.position
    );
}

bool sphereIntersect(Sphere sphere, Ray ray, out Intersection intersection) {
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

bool triangleIntersect(Triangle triangle, Ray ray, out Intersection intersection) {
    vec3 v0v1 = triangle.vertex2 - triangle.vertex1;
    vec3 v0v2 = triangle.vertex3 - triangle.vertex1;
    vec3 pVec = cross(ray.direction, v0v2);
    float det = dot(v0v1, pVec);
    if (abs(det) < 1e-8) {
        return false;
    }
    float invDet = 1 / det;
    vec3 tVec = ray.origin - triangle.vertex1;
    float u = dot(tVec, pVec) * invDet;
    if (u < 0 || u > 1) {
        return false;
    }
    vec3 qVec = cross(tVec, v0v1);
    float v = dot(ray.direction, qVec) * invDet;
    if (v < 0 || u + v > 1) {
        return false;
    }
    float t = dot(v0v2, qVec) * invDet;
    if (t < 0) {
        return false;
    }
    intersection.distance = t;
    intersection.normal = normalize(v0v1 * v0v2);
    intersection.point = ray.direction * t + ray.origin;
    return true;
}

bool intersect(Primitive primitive, Ray ray, out Intersection intersection) {
    if (primitive.type == 0) {
        return sphereIntersect(
            Sphere(primitive.spherePosition, primitive.sphereRadiusSquared),
            ray, intersection
        );
    }
    return triangleIntersect(
        Triangle(
            primitive.triangleVertex1, primitive.triangleVertex2,
            primitive.triangleVertex3
        ), ray, intersection
    );
}

vec3 phongIlluminate(
    Intersection intersection, Light light, Material material, Ray ray
) {
    vec3 L = normalize(light.position - intersection.point);
    float LdotN = max(0, dot(L, intersection.normal));
    vec3 c = material.colour * LdotN * material.diffuse;
    if (LdotN > 0 && material.specular > 0) {
        vec3 R = normalize(intersection.normal * 2.0 * LdotN - L);
        vec3 V = normalize(ray.origin - intersection.point);
        c += light.colour * pow(max(0.0, dot(R, V)), material.shininess) * material.specular;
    }
    return c;
}