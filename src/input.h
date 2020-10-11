#ifndef __INPUT_H__
#define __INPUT_H__

#include "stdio.h"
#include <SDL.h>
#include <stdbool.h>


//https://wiki.libsdl.org/SDLScancodeLookup

void input_init();

bool input_isKeyPressed(SDL_Scancode key);

void input_mouse_offset(int * delta_x, int * delta_y);


#endif