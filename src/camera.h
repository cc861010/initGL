#ifndef _camera_h_
#define _camera_h_

#include <stdio.h>
#include <cglm/cglm.h>
#include <math.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} Camera_Movement;

typedef struct camera {
    // camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    //view matrix
    mat4 view;
} camera;

camera * camera_init(vec3 position, vec3 up, float * yaw, float * pitch);

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void camera_processKeyboard(camera *camera, enum Camera_Movement direction, float deltaTime);

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void camera_processMouseMovement(camera *camera,float x_offset, float y_offset, bool constrainPitch);

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void camera_processMouseScroll(camera *camera,float y_offset);

void camera_free(camera *camera);

#endif