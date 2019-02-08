#include <SDL2/SDL.h>
#include "scene.h"

struct Renderer {

    virtual ~Renderer() = default;
    virtual void render(const Scene &scene) = 0;

protected:

    SDL_Window *window;

    Renderer(SDL_Window *window);

};

Renderer *createRenderer(SDL_Window *window);