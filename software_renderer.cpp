#include <cmath>
#include <iostream>
#include "illumination.h"
#include "renderer.h"
#include "timer.h"

static Vector rayTrace(Primitive *ignore, const Ray &ray, const Scene &scene) {
    Intersection closestIntersection = NO_INTERSECTION;
    Illumination *illumination;
    Material material;
    Primitive *primitive;
    for (auto i = 0U; i < scene.primitives.size(); i++) {
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
    Vector colour = scene.ambient;
    for (auto &light : scene.lights) {
        auto intersectsPrimitive = false;
        auto lightRay = Ray{
            (light.position - closestIntersection.point).normalise(),
            closestIntersection.point
        };
        for (auto &p : scene.primitives) {
            if (primitive != p && !std::isinf(p->intersect(lightRay).distance)) {
                intersectsPrimitive = true;
                break;
            }
        }
        if (!intersectsPrimitive) {
            colour += illumination->illuminate(
                closestIntersection, light, material, ray
            );
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
        SDL_LockTexture(
            texture, nullptr, reinterpret_cast<void **>(&pixels), &pitch
        );
        auto aspectRatio = width * 1.0 / height;
        Timer::start();
        #pragma omp parallel for collapse(2)
        for (auto x = 0; x < width; x++) {
            for (auto y = 0; y < height; y++) {
                pixels[x + y * width] = static_cast<unsigned>(rayTrace(nullptr, scene.camera.castRay(((x * 1.0 / width) - 0.5) * aspectRatio, y * 1.0 / height - 0.5), scene));
            }
        }
        auto time1 = Timer::takeTime();
        SDL_UnlockTexture(texture);
        auto time2 = Timer::takeTime();
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        auto time3 = Timer::takeTime();
        SDL_RenderPresent(renderer);
        auto time4 = Timer::takeTime();
        std::cout << "Render timings:" << std::endl;
        std::cout << "Pixel rendering\t\t" << time1 << std::endl;
        std::cout << "SDL_UnlockTexture\t" << (time2 - time1) << std::endl;
        std::cout << "SDL_RenderCopy\t\t" << (time3 - time2) << std::endl;
        std::cout << "SDL_RenderPresent\t" << (time4 - time3) << std::endl;
        std::cout << "Total rendering\t\t" << time4 << std::endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
    }

};

Renderer *createRenderer() {
    return new SoftwareRenderer;
}