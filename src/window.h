#ifndef __WINDOW_H__
#define __WINDOW_H__

typedef void * (* t_before_init )();
typedef (* t_initialize )(void *);
typedef (* t_before_render)(void *);
typedef (* t_render)(void *);
typedef (* t_finish)(void *);

int window(t_before_init, t_initialize, t_before_render, t_render, t_finish);

#endif