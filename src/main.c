
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

void make_block_vertex_288f_36c(
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
    GLuint shader_program;
    camera *data_camera;
    GLuint VAO;
    GLsizei count;
    GLuint data_texture;
} context;

void *init() {
    context c = {0};
    context *variable = malloc(sizeof(context));
    memcpy(variable, &c, sizeof(context));

    //shader
    variable->shader_program = shader_programs_load("resource/7.1.camera.craft.vs", "resource/7.1.camera.craft.fs");
    shader_use(variable->shader_program);

    //vertex
    vertex_attribute *vertex_attr = vertex_layout_create(9, "position", 3, sizeof(float), "normal", 3, sizeof(float),"uv", 2, sizeof(float));

    //////////////////////magic begin///////////////////////////////////////////////////////////////////////////////
    float vertices[288*6] = {0};

    make_block_vertex_288f_36c(vertices, true, true, true, true, true, true, 16, 16, 32, 0, 16, 16, 0, 0, 0);
    make_block_vertex_288f_36c(vertices + 288*1, true, true, true, true, true, true, 2, 2, 2, 2, 2, 2, 1, 0, 1);
    make_block_vertex_288f_36c(vertices + 288*2, true, true, true, true, true, true, 1, 1, 1, 1, 1, 1, 0, 0, 1);
    make_block_vertex_288f_36c(vertices + 288*3, true, true, true, true, true, true, 1, 1, 1, 1, 1, 1, 2, 0, 1);
    make_block_vertex_288f_36c(vertices + 288*4, true, true, true, true, true, true, 1, 1, 1, 1, 1, 1, 1, 0, 2);
    make_block_vertex_288f_36c(vertices + 288*5, true, true, true, true, true, true, 16, 16, 32, 0, 16, 16, 2, 0, 1);

    variable->VAO = vertex_data_load(variable->shader_program, vertex_attr, vertices, sizeof(vertices));
    variable->count = 36 *6 ;

    ///////////////////////magic end//////////////////////////////////////////////////////////////////////////////

    //texture
    variable->data_texture = texture_create("resource/texture.png");
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
    //model matrix
    mat4 model = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    shader_setMat4(variable->shader_program, "model", model);
    //camera
    variable->data_camera = camera_init(NULL, NULL, NULL, NULL);
    //input
    input_init();

    glEnable(GL_DEPTH_TEST);

    return variable;
}

void update(void *data, Uint32 delta_time, SDL_Event *event) {
    context *variable = (context *) data;

    if (input_isKeyPressed(SDL_SCANCODE_A)) {
        camera_processKeyboard(variable->data_camera, LEFT, delta_time);
    }
    if (input_isKeyPressed(SDL_SCANCODE_W)) {
        camera_processKeyboard(variable->data_camera, FORWARD, delta_time);
    }
    if (input_isKeyPressed(SDL_SCANCODE_S)) {
        camera_processKeyboard(variable->data_camera, BACKWARD, delta_time);
    }
    if (input_isKeyPressed(SDL_SCANCODE_D)) {
        camera_processKeyboard(variable->data_camera, RIGHT, delta_time);
    }
    int dx, dy;
    input_mouse_offset(&dx, &dy);

    camera_processMouseMovement(variable->data_camera, dx, dy, true);
    shader_setMat4(variable->shader_program, "view", variable->data_camera->view);

}

void render(void *data, Uint32 delta_time) {
    context *variable = (context *) data;
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    texture_use(variable->data_texture);
    //draw
    glBindVertexArray(variable->VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, variable->count);

}

void finish(void *data) {
    context *variable = (context *) data;
    free(variable);
}

int main(int argc, char *argv[]) {
    return window(init, update, render, finish);
}