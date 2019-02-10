#include "illumination.h"
#include "renderer.h"

static Vector rayTrace(Primitive *ignore, const Ray &ray, const Scene &scene) {
    Intersection closestIntersection = NO_INTERSECTION;
    Illumination *illumination;
    Material material;
    Primitive *primitive;
    for (auto i = 0; i < scene.primitives.size(); i++) {
        if (ignore != scene.primitives[i]) {
            Intersection intersection = scene.primitives[i]->intersect(ray);
            if (!std::isinf(intersection.distance) && intersection.distance < closestIntersection.distance) {
                closestIntersection = intersection;
                illumination = scene.illuminations[i];
                material = scene.materials[i];
                primitive = scene.primitives[i];
            }
        }
    }
    if (std::isinf(closestIntersection.distance)) {
        return {};
    }
    Vector colour;
    for (auto &light : scene.lights) {
        auto intersectsPrimitive = false;
        auto ray = Ray{(light.position - closestIntersection.point).normalise(), closestIntersection.point};
        for (auto &p : scene.primitives) {
            if (primitive != p && !std::isinf(p->intersect(ray).distance)) {
                intersectsPrimitive = true;
                break;
            }
        }
        if (!intersectsPrimitive) {
            colour += illumination->illuminate(closestIntersection, light, material);
        }
    }
    if (material.reflectivity > 0) {
        colour += rayTrace(primitive, {ray.direction - closestIntersection.normal * 2.0 * closestIntersection.normal.dot(ray.direction), closestIntersection.point}, scene) * material.reflectivity;
    }
    return colour;
}

struct SoftwareRenderer : Renderer {

    void render(SDL_Window *window, const Scene &scene) const {
        int height, pitch, width;
        SDL_GetWindowSize(window, &width, &height);
        SDL_Renderer *renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED
        );
        SDL_Texture *texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
            width, height
        );
        unsigned *pixels;
        SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&pixels), &pitch);
        auto aspectRatio = width * 1.0 / height;
        #pragma omp parallel for collapse(2)
        for (auto x = 0; x < width; x++) {
            for (auto y = 0; y < height; y++) {
                pixels[x + y * width] = static_cast<unsigned>(rayTrace(nullptr, scene.camera.trace(x * 1.0 / width * aspectRatio - 0.5, y * 1.0 / height - 0.5), scene));
            }
        }
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
    }

};

Renderer *createRenderer() {
    return new SoftwareRenderer;
}