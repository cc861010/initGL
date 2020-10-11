
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

void make_vertex(
        float *data, //float ao[6][4], float light[6][4],
        bool left, bool right, bool top, bool bottom, bool front, bool back,
        int wleft, int wright, int wtop, int wbottom, int wfront, int wback,
        float x, float y, float z) {
    float n = 0.5;
    static const float positions[6][4][3] = {
            {{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1}},
            {{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}},
            {{-1, +1, -1}, {-1, +1, +1}, {+1, +1, -1}, {+1, +1, +1}},
            {{-1, -1, -1}, {-1, -1, +1}, {+1, -1, -1}, {+1, -1, +1}},
            {{-1, -1, -1}, {-1, +1, -1}, {+1, -1, -1}, {+1, +1, -1}},
            {{-1, -1, +1}, {-1, +1, +1}, {+1, -1, +1}, {+1, +1, +1}}
    };
    static const float normals[6][3] = {
            {-1, 0,  0},
            {+1, 0,  0},
            {0,  +1, 0},
            {0,  -1, 0},
            {0,  0,  -1},
            {0,  0,  +1}
    };
    static const float uvs[6][4][2] = {
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 0}, {0, 0}, {1, 1}, {0, 1}},
            {{0, 1}, {0, 0}, {1, 1}, {1, 0}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
            {{1, 0}, {1, 1}, {0, 0}, {0, 1}}
    };
    static const float indices[6][6] = {
            {0, 3, 2, 0, 1, 3},
            {0, 3, 1, 0, 2, 3},
            {0, 3, 2, 0, 1, 3},
            {0, 3, 1, 0, 2, 3},
            {0, 3, 2, 0, 1, 3},
            {0, 3, 1, 0, 2, 3}
    };
    static const float flipped[6][6] = {
            {0, 1, 2, 1, 3, 2},
            {0, 2, 1, 2, 3, 1},
            {0, 1, 2, 1, 3, 2},
            {0, 2, 1, 2, 3, 1},
            {0, 1, 2, 1, 3, 2},
            {0, 2, 1, 2, 3, 1}
    };
    float *d = data;
    float s = 0.0625; // 1/16
    float a = 0 + 1 / 2048.0;
    float b = s - 1 / 2048.0;
    bool faces[6] = {left, right, top, bottom, front, back};
    int tiles[6] = {wleft, wright, wtop, wbottom, wfront, wback};
    for (int i = 0; i < 6; i++) {
        if (faces[i] == false) {
            continue;
        }
        float du = (tiles[i] % 16) * s;
        float dv = (tiles[i] / 16) * s;
        //int flip = ao[i][0] + ao[i][3] > ao[i][1] + ao[i][2];
        int flip = 0;
        for (int v = 0; v < 6; v++) {
            int j = flip ? flipped[i][v] : indices[i][v];
            *(d++) = x + n * positions[i][j][0];
            *(d++) = y + n * positions[i][j][1];
            *(d++) = z + n * positions[i][j][2];
            *(d++) = normals[i][0];
            *(d++) = normals[i][1];
            *(d++) = normals[i][2];
            *(d++) = du + (uvs[i][j][0] ? b : a);
            *(d++) = dv + (uvs[i][j][1] ? b : a);
            //*(d++) = ao[i][j];
            //*(d++) = light[i][j];
        }
    }
}

typedef struct {
    void * cube_position;
    float *data_buffer;
    unsigned int data_buffer_size;
    int data_count;
    GLuint data_texture;
    GLuint data_texture1;
    camera * data_camera;
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

    variable->shader_program = shader_programs_load("resource/7.1.camera.craft.vs", "resource/7.1.camera.craft.fs");
    float vertices[6*6*8] = {0};
    make_vertex(vertices,true,true,true,true,true,true,16, 16, 32, 0, 16, 16,0,0,0);

    vertex_attribute *vertex_attr = vertex_layout_create(9, "position", 3, sizeof(float),"normal", 3, sizeof(float), "uv", 2, sizeof(float));
    variable->VAO = vertex_data_load(variable->shader_program, vertex_attr, vertices, sizeof(vertices));

    variable->data_texture = texture_create("resource/texture.png");

    shader_use(variable->shader_program);
    shader_setInt(variable->shader_program, "texture1", variable->data_texture);

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

    variable->data_camera = camera_init(NULL, NULL, NULL, NULL);
    shader_setMat4(variable->shader_program, "view", variable->data_camera->view);

    glEnable(GL_DEPTH_TEST);

    input_init();

    return variable;
}

void update(void * data, Uint32 delta_time,SDL_Event * event){
    context *variable = (context *) data;

    if(input_isKeyPressed(SDL_SCANCODE_A)){
        camera_processKeyboard(variable->data_camera,LEFT,delta_time);
    }
    if(input_isKeyPressed(SDL_SCANCODE_W)){
        camera_processKeyboard(variable->data_camera,FORWARD,delta_time);
    }
    if(input_isKeyPressed(SDL_SCANCODE_S)){
        camera_processKeyboard(variable->data_camera,BACKWARD,delta_time);
    }
    if(input_isKeyPressed(SDL_SCANCODE_D)){
        camera_processKeyboard(variable->data_camera,RIGHT,delta_time);
    }
    int dx,dy;
    input_mouse_offset(&dx,&dy);

    camera_processMouseMovement(variable->data_camera,dx,dy,true);
    shader_setMat4(variable->shader_program, "view", variable->data_camera->view);

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
//        float angle = 20.0f * i;
//        glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 1.3f, 0.5f});
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