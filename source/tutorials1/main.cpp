
#include <SDL.h>
#include <stdio.h>
#include "angle_gl.h"

const unsigned int DISP_WIDTH = 800;
const unsigned int DISP_HEIGHT = 600;

int main(int argc, char *argv[]) {

    // 1. Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //Setup the exit hook
    atexit(SDL_Quit);

    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 2. Create the window
    SDL_Window *window =  SDL_CreateWindow("SDL2",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't creat the main window", NULL);
        return EXIT_FAILURE;
    }

    // 3. Create the OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an opengl context", NULL);
        return EXIT_FAILURE;
    }

    //Clear to black
    glClearColor(1.0f, 1.0f, 0.5f , 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Update the window
    SDL_GL_SwapWindow(window);

    // Wait for the user to quit
    bool quit = false;
    while(!quit){
        SDL_Event event;
        if(SDL_WaitEvent(&event)!=0){
            if(event.type == SDL_QUIT){
                quit = true;
            }
        }
    }
    return EXIT_SUCCESS;

}