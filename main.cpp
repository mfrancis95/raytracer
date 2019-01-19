#include "renderer.h"

int main() {
    auto window = SDL_CreateWindow(
        "Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960,
        0
    );
    Renderer *renderer = createRenderer(window);
    renderer->render();
    SDL_Event event;
    auto quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
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