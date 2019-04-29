#include "group.h"
#include "obj.h"
#include "phong.h"
#include "renderer.h"
#include "sphere.h"
#include "timer.h"
#include "triangle.h"

int main() {
    auto window = SDL_CreateWindow(
        "Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960,
        SDL_WINDOW_OPENGL
    );
    auto renderer = createRenderer();
    Scene scene = {0x11};
    // Bunny
    /*scene.camera = {{0, 0, 0.25}, {0, 0.1, 0}};
    Timer::start();
    scene.primitives = OBJ::read("bunny.obj");
    std::cout << "Read bunny\t\t" << Timer::takeTime() << std::endl;*/
    // Cube
    scene.camera = {{2, 3, 5}, {0, 0, 0}};
    Timer::start();
    scene.primitives = OBJ::read("cube.obj");
    std::cout << "Read cube\t\t" << Timer::takeTime() << std::endl;
    // Teapot
    /*scene.camera = {{2, 5, 10}, {0, 0, 0}};
    Timer::start();
    scene.primitives = OBJ::read("teapot.obj");
    std::cout << "Read teapot\t\t" << Timer::takeTime() << std::endl;*/
    scene.lights.push_back({0xFFFFFF, {5, 3, 2}});
    for (auto i = 0; i < scene.primitives.size(); i++) {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0xFF0000, 1});
    }
    /*{
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0xFF0000, 1});
        scene.primitives.push_back(new Sphere{{0, -1, -3}, 0.25});
    }
    {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0xFF000, 0.5, 0, 5, 0.5});
        scene.primitives.push_back(new Sphere{{1, -0.5, -3.5}, 0.25});
    }
    {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0x555555, 1, 0.5});
        scene.primitives.push_back(new Triangle{{-2, -1, -6}, {-2, -1, -2}, {2, -1, -2}});
    }
    {
        scene.illuminations.push_back(new Phong);
        scene.materials.push_back({0x555555, 1, 0.5});
        scene.primitives.push_back(new Triangle{{2, -1, -6}, {-2, -1, -6}, {2, -1, -2}});
    }*/
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
