#include "input.h"

typedef struct input_state {
    const Uint8 *keyboard;
    int last_frame_mouse_x;
    int last_frame_mouse_y
} input_state;

extern input_state *_this = NULL;

void input_init() {
    _this = malloc(sizeof(input_state));
}

bool input_isKeyPressed(SDL_Scancode key) {
    Uint8 * key_state = SDL_GetKeyboardState(NULL);
    return key_state[key];
}

void input_mouse_offset(int * dx, int * dy){
    int x,y;
    SDL_GetMouseState(&x, &y);
    * dx = x - _this->last_frame_mouse_x;
    * dy = y - _this->last_frame_mouse_y;
    _this->last_frame_mouse_x = x;
    _this->last_frame_mouse_y = y;
}