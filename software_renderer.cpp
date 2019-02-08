#include <iostream>
#include "renderer.h"

struct SoftwareRenderer : Renderer {

    SoftwareRenderer(SDL_Window *window) : Renderer{window} {
        SDL_GetWindowSize(window, &width, &height);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    ~SoftwareRenderer() {
        SDL_DestroyRenderer(renderer);
    }

    void render(const Scene &scene) {
        SDL_Texture *texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
            width, height
        );
        int pitch, *pixels;
        SDL_LockTexture(texture, nullptr, (void **) &pixels, &pitch);
        std::cout << scene.camera.right << std::endl;
        for (auto x = 0; x < width; x++) {
            for (auto y = 0; y < height; y++) {
                Intersection closestIntersection = NO_INTERSECTION;
                const Primitive *closestPrimitive;
                auto ray = scene.camera.trace((double) x, (double) y);
                for (auto &primitive : scene.primitives) {
                    Intersection intersection = primitive->intersect(ray);
                    if (!std::isinf(intersection.distance) && intersection.distance < closestIntersection.distance) {
                        closestIntersection = intersection;
                        closestPrimitive = primitive;
                    }
                }
                if (!std::isinf(closestIntersection.distance)) {
                    pixels[x + y * width] = 0xFF0000;
                }
            }
        }
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }

private:

    int height, width;
    SDL_Renderer *renderer;

    friend Renderer *createRenderer(SDL_Window *window);

};

Renderer *createRenderer(SDL_Window *window) {
    return new SoftwareRenderer{window};
}