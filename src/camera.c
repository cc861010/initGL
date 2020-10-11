#include "camera.h"
#include "string.h"

// Default camera values
const float YAW =  0.0f;
const float PITCH = 0.0f;
const float SPEED = 0.001f;
const float MOUSE_SENSITIVITY = 0.09f;
const float ZOOM = 45.0f;

void _update_viewMatrix(camera *camera) {
    float target[] = {0.0f, 0.0f, 0.0f};
    glm_vec3_add(camera->Position, camera->Front, target);
    printf("eye:(%f,%f,%f),target:(%f,%f,%f),up:(%f,%f,%f)\n",
           camera->Position[0], camera->Position[1], camera->Position[2],
           target[0], target[1], target[2],
           camera->Up[0], camera->Up[1], camera->Up[2]);
    glm_lookat(camera->Position, target, camera->Up, &(camera->view));
}

// calculates the front vector from the Camera's (updated) Euler Angles
void updateCameraVectors(camera *camera) {
    // calculate the new Front vector
    camera->Front[0] = cosf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch));
    camera->Front[1] = sinf(glm_rad(camera->Pitch));
    camera->Front[2] = sinf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch));
    glm_vec3_norm(camera->Front);
    //Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    glm_vec3_cross(camera->Front, camera->WorldUp, camera->Right);
    glm_vec3_cross(camera->Right, camera->Front, camera->Up);
    glm_vec3_norm(camera->Up);
    _update_viewMatrix(camera);
}

void camera_init(camera *camera, vec3 position, vec3 up, float *yaw, float *pitch) {
    camera->Front[0] = 0.0f;
    camera->Front[1] = 0.0f;
    camera->Front[2] = -1.0f;
    camera->MovementSpeed = SPEED;
    camera->MouseSensitivity = MOUSE_SENSITIVITY;
    camera->Zoom = ZOOM;
    //init position
    if (position == NULL) {
        camera->Position[0] = 0.0f;
        camera->Position[1] = 0.0f;
        camera->Position[2] = 0.0f;
    } else {
        camera->Position[0] = position[0];
        camera->Position[1] = position[1];
        camera->Position[2] = position[2];
    };
    //init up
    if (up == NULL) {
        camera->WorldUp[0] = 0.0f;
        camera->WorldUp[1] = 1.0f;
        camera->WorldUp[2] = 0.0f;
    } else {
        camera->WorldUp[0] = up[0];
        camera->WorldUp[1] = up[1];
        camera->WorldUp[2] = up[2];
    }
    //init Yaw
    camera->Yaw = (yaw == NULL ? YAW : *yaw);
    //init Pitch
    camera->Pitch = (pitch == NULL ? PITCH : *pitch);
    //init view matrix
    mat4 view = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    memcpy(camera->view, &view, sizeof(view));
    updateCameraVectors(camera);
}


// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void camera_processKeyboard(camera *camera, enum Camera_Movement direction, float deltaTime) {
    float velocity = camera->MovementSpeed * deltaTime;
    vec3 delta = {0, 0, 0};
    if (direction == FORWARD) {
        glm_vec3_scale(camera->Front, velocity, delta);
        glm_vec3_add(camera->Position, delta, camera->Position);
    }
    if (direction == BACKWARD) {
        glm_vec3_scale(camera->Front, velocity, delta);
        glm_vec3_sub(camera->Position, delta, camera->Position);
    }
    if (direction == LEFT) {
        glm_vec3_scale(camera->Right, velocity, delta);
        glm_vec3_sub(camera->Position, delta, camera->Position);
    }
    if (direction == RIGHT) {
        glm_vec3_scale(camera->Right, velocity, delta);
        glm_vec3_add(camera->Position, delta, camera->Position);
    }
    updateCameraVectors(camera);
}


// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void camera_processMouseMovement(camera *camera, float x_offset, float y_offset, bool constrainPitch) {
    x_offset *= camera->MouseSensitivity;
    y_offset *= camera->MouseSensitivity;

    camera->Yaw += x_offset;
    camera->Pitch += -y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (camera->Pitch > 89.0f)
            camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f)
            camera->Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors(camera);
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void camera_processMouseScroll(camera *camera, float y_offset) {
    camera->Zoom -= (float) y_offset;
    if (camera->Zoom < 1.0f)
        camera->Zoom = 1.0f;
    if (camera->Zoom > 45.0f)
        camera->Zoom = 45.0f;
    updateCameraVectors(camera);
}