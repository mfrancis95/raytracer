#include "group.h"
#include "phong.h"
#include "renderer.h"
#include "sphere.h"
#include "triangle.h"

int main() {
    auto window = SDL_CreateWindow(
        "Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960,
        0
    );
    auto renderer = createRenderer();
    Scene scene;
    scene.lights.push_back({{4, 2, 0}});
    {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0xFF0000});
        scene.primitives.push_back(new Sphere{{0, -1, -3}, 0.25});
    }
    {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0xFF00});
        scene.primitives.push_back(new Sphere{{1, 0, -3.5}, 0.5});
    }
    {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0xFF});
        scene.primitives.push_back(new Group{{
            new Triangle{{-2, -1, -6}, {-2, -1, -2}, {2, -1, -2}},
            new Triangle{{2, -1, -6}, {-2, -1, -6}, {2, -1, -2}}
        }});
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