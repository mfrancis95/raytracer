#include "renderer.h"
#include "sphere.h"

int main() {
    auto window = SDL_CreateWindow(
        "Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960,
        0
    );
    auto renderer = createRenderer(window);
    Scene scene = {Camera{{0, 0, 3}, {0, 0, 0}}};
    scene.primitives.push_front(new Sphere{{0, 0, -2}, 1});
    renderer->render(scene);
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