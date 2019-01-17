#include <SDL2/SDL.h>

struct Renderer {

    virtual ~Renderer() = default;
    virtual void render() = 0;

protected:

    SDL_Window *window;

    Renderer(SDL_Window *window);

};

Renderer *createRenderer(SDL_Window *window);