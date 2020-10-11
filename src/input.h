#ifndef __INPUT_H__
#define __INPUT_H__

#include "stdio.h"
#include <SDL.h>
#include <stdbool.h>


//https://wiki.libsdl.org/SDLScancodeLookup

#define KEYBOARD_SIZE 282
#define MOUSE_MAX 6

typedef struct input_state {
    const Uint8 *keyboard;
    int last_frame_mouse_x;
    int last_frame_mouse_y

} input_state;

input_state *input_init();

bool input_isKeyPressed(input_state *this, SDL_Scancode key);

void input_mouse_delta(input_state *this,int * delta_x, int * delta_y);


#endif