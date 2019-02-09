#include "illumination.h"
#include "renderer.h"

struct SoftwareRenderer : Renderer {

    void render(SDL_Window *window, const Scene &scene) {
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
        SDL_LockTexture(texture, nullptr, (void **) &pixels, &pitch);
        auto aspectRatio = width * 1.0 / height;
        #pragma omp parallel for collapse(2)
        for (auto x = 0; x < width; x++) {
            for (auto y = 0; y < height; y++) {
                Intersection closestIntersection = NO_INTERSECTION;
                Illumination *illumination;
                Material material;
                Primitive *primitive;
                auto ray = scene.camera.trace(x * 1.0 / width * aspectRatio - 0.5, y * 1.0 / height - 0.5);
                for (auto i = 0; i < scene.primitives.size(); i++) {
                    Intersection intersection = scene.primitives[i]->intersect(ray);
                    if (!std::isinf(intersection.distance) && intersection.distance < closestIntersection.distance) {
                        closestIntersection = intersection;
                        illumination = scene.illuminations[i];
                        material = scene.materials[i];
                        primitive = scene.primitives[i];
                    }
                }
                if (!std::isinf(closestIntersection.distance)) {
                    Vector colour;
                    for (auto &light : scene.lights) {
                        colour += illumination->illuminate(closestIntersection, light, material);
                    }
                    pixels[x + y * width] = (unsigned) colour;
                }
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