#include <SDL.h>
#include <stddef.h>
#include <stdbool.h>
#include "window.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int window(t_init init, t_update  _update, t_render _render, t_finish _finish) {

    // 1. Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //Setup the exit hook
    //atexit(SDL_Quit);

    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    //Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 2. Create the window
    SDL_Window *window = SDL_CreateWindow("SDL2",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't creat the main window", NULL);
        return EXIT_FAILURE;
    }

    // 3. Create the OpenGL context
    if (!SDL_GL_CreateContext(window)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an opengl context", NULL);
        return EXIT_FAILURE;
    }

    void * data = init();

    bool quit = false;
    Uint32 last_time =SDL_GetTicks(), current_time=0 ,delta_time=0;

    SDL_Event event;
    while (!quit) {
        while(SDL_PollEvent( &event )){
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    _update(data,delta_time,&event);
            }
        }

        _render(data,delta_time);
        SDL_GL_SwapWindow(window);

        current_time = SDL_GetTicks();
        delta_time = current_time - last_time;
        last_time = current_time;
    }
    _finish(data);
    return EXIT_SUCCESS;
}


