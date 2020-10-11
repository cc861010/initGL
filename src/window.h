#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "SDL_events.h"

typedef void * (* t_init )();
typedef (* t_update)(void *,Uint32 delta_time, SDL_Event *);
typedef (* t_render)(void *,Uint32 delta_time);
typedef (* t_finish)(void *);

int window(t_init, t_update, t_render, t_finish);

#endif