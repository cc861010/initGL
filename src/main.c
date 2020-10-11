
#include <SDL.h>
#include "angle_gl.h"
#include "shader.h"
#include "texture.h"
#include <cglm/cglm.h>
#include <stb_image.h>
#include "window.h"
#include "string.h"
#include "vertex.h"
#include "camera.h"
#include "input.h"


typedef struct {
    input_state * input;
    void * cube_position;
    float *data_buffer;
    unsigned int data_buffer_size;
    int data_count;
    GLuint data_texture;
    GLuint data_texture1;
    camera data_camera;
    mat4 data_model;
    mat4 data_view;
    mat4 data_projection;

    GLuint shader_program;

    GLuint VAO;

    GLuint vertex_position;
    GLuint vertex_color;
    GLuint vertex_uv;

    GLuint uniform_camera;
    GLuint uniform_model;
    GLuint uniform_view;
    GLuint uniform_projection;
    GLuint uniform_sampler;
} context;

void * init() {
    context c = {0};
    context *variable = malloc(sizeof(context));
    memcpy(variable, &c, sizeof(context));

    variable->shader_program = shader_programs_load("resource/7.1.camera.vs", "resource/7.1.camera.fs");
    float vertices[] = {
            //position and texture coord attribute
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    vertex_attribute *vertex_attr = vertex_layout_create(6, "aPos", 3, sizeof(float), "aTexCoord", 2, sizeof(float));
    variable->VAO = vertex_data_load(variable->shader_program, vertex_attr, vertices, sizeof(vertices));

    variable->data_texture1 = texture_create("resource/awesomeface.png");
    variable->data_texture = texture_create("resource/container2.png");

    shader_use(variable->shader_program);
    shader_setInt(variable->shader_program, "texture1", variable->data_texture);
    shader_setInt(variable->shader_program, "texture2", variable->data_texture1);

    //projection matrix
    mat4 projection = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    glm_perspective(glm_rad(30.0f), (float) 800 / (float) 600, 0.1f, 100.0f, projection);


    shader_setMat4(variable->shader_program, "projection", projection);

    vec3 cubePositions[] = {
            {0.0f,  0.0f,  0.0f},
            {2.0f,  5.0f,  -15.0f},
            {-1.5f, -2.2f, -2.5f},
            {-3.8f, -2.0f, -12.3f},
            {2.4f,  -0.4f, -3.5f},
            {-1.7f, 3.0f,  -7.5f},
            {1.3f,  -2.0f, -2.5f},
            {1.5f,  2.0f,  -2.5f},
            {1.5f,  0.2f,  -1.5f},
            {-1.3f, 1.0f,  -1.5f}};

    variable->cube_position = cubePositions;

    camera_init(&(variable->data_camera), NULL, NULL, NULL, NULL);
    shader_setMat4(variable->shader_program, "view", variable->data_camera.view);

    glEnable(GL_DEPTH_TEST);

    variable->input = input_init();

    return variable;
}


void update(void * data, Uint32 delta_time,SDL_Event * event){
    context *variable = (context *) data;

    if(input_isKeyPressed(variable->input,SDL_SCANCODE_A)){
        camera_processKeyboard(&(variable->data_camera),LEFT,delta_time);
    }
    if(input_isKeyPressed(variable->input,SDL_SCANCODE_W)){
        camera_processKeyboard(&(variable->data_camera),FORWARD,delta_time);
    }
    if(input_isKeyPressed(variable->input,SDL_SCANCODE_S)){
        camera_processKeyboard(&(variable->data_camera),BACKWARD,delta_time);
    }
    if(input_isKeyPressed(variable->input,SDL_SCANCODE_D)){
        camera_processKeyboard(&(variable->data_camera),RIGHT,delta_time);
    }

    int dx,dy;
    input_mouse_delta(variable->input,&dx,&dy);
    camera_processMouseMovement(&(variable->data_camera),dx,dy,true);
    shader_setMat4(variable->shader_program, "view", variable->data_camera.view);

}


void render(void *data, Uint32 delta_time) {
    context *variable = (context *) data;


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // bind textures on corresponding texture units

    texture_use(variable->data_texture);
    texture_use(variable->data_texture1);
    //draw
    glBindVertexArray(variable->VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    vec3 * cubePositions = (vec3*)variable->cube_position;

    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        //model matrix
        mat4 model = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        };
        vec3 position = {};
        glm_vec3_scale(cubePositions[i],3,position);
        glm_translate(model, position);
        float angle = 20.0f * i;
        glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 1.3f, 0.5f});
        shader_setMat4(variable->shader_program, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}

void finish(void *data) {
    context *variable = (context *) data;
    free(variable);
}

int main(int argc, char *argv[]) {
    return window(init, update, render, finish);
}