#include <SDL2/SDL.h>
#include "scene.h"

struct Renderer {

    virtual void render(SDL_Window *window, const Scene &scene) const = 0;

};

Renderer *createRenderer();