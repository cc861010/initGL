#include "input.h"

input_state *input_init() {
    input_state *this = malloc(sizeof(input_state));
}


bool input_isKeyPressed(input_state *this, SDL_Scancode key) {
    Uint8 * key_state = SDL_GetKeyboardState(NULL);
    return key_state[key];
}

void input_mouse_delta(input_state *this,int * dx, int * dy){
    int x,y;
    SDL_GetMouseState(&x, &y);
    * dx = x - this->last_frame_mouse_x;
    * dy = y - this->last_frame_mouse_y;
    this->last_frame_mouse_x = x;
    this->last_frame_mouse_y = y;
}