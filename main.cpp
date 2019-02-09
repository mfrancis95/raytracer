#include "phong.h"
#include "renderer.h"
#include "sphere.h"

int main() {
    auto window = SDL_CreateWindow(
        "Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960,
        0
    );
    auto renderer = createRenderer();
    Scene scene;
    scene.lights.push_back({0xFFFFFF});
    {
        scene.illuminations.push_back(new Phong{});
        scene.materials.push_back({0xFF0000});
        scene.primitives.push_back(new Sphere{{0, 0, -5}, 1});
    }
    {
        scene.illuminations.push_back(new Phong{});
        scene.materials.push_back({0x00FF00});
        scene.primitives.push_back(new Sphere{{1, 0, -5}, 0.5});
    }
    renderer->render(window, scene);
    SDL_Event event;
    auto quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
        }
        SDL_Delay(16);
    }
    delete renderer;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}