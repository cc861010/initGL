
#include <SDL.h>
#include "angle_gl.h"
#include "shader.h"
#include "texture.h"
#include <cglm/cglm.h>   /* for inline */
#include <cglm/call.h>   /* for library call (this also includes cglm.h) */
#include <stb_image.h>
#include "window.h"
#include "string.h"
#include "vertex.h"


typedef struct {
    float *data_buffer;
    unsigned int data_buffer_size;
    int data_count;
    GLuint data_texture;
    GLuint data_texture1;
    vec3 data_camera;
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


void *before_init() {
    context c = {0};
    void *r = malloc(sizeof(context));
    memcpy(r, &c, sizeof(context));
    return r;
}

void init(void *data) {
    context *variable = (context *) data;
    variable->shader_program = shader_programs_load("resource/4.2.texture.vert", "resource/4.2.texture.frag");
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    vertex_attribute *vertex_attr = vertex_layout_create(9, "aPos", 3, sizeof(float), "aColor", 3, sizeof(float),
                                                         "aTexCoord", 2, sizeof(float));
    variable->VAO = vertex_data_load(variable->shader_program, vertex_attr, vertices, sizeof(vertices), indices,
                                     sizeof(indices));

    variable->data_texture1 = texture_load("resource/awesomeface.png");
    variable->data_texture = texture_load("resource/mine_texture.png");

}

void before_render(void *data) {
    context *variable = (context *) data;
    // activate shader
    glUseProgram(variable->shader_program);
    glUniform1i(glGetUniformLocation(variable->shader_program, "texture1"), variable->data_texture);
    glUniform1i(glGetUniformLocation(variable->shader_program, "texture2"), variable->data_texture1);
    //set uniform variable
//    glUniform3f(attrib->uniform_camera, attrib->data_camera[0], attrib->data_camera[1], attrib->data_camera[2]);
//    glUniformMatrix4fv(attrib->uniform_model, 1, GL_FALSE, attrib->data_model);
//    glUniformMatrix4fv(attrib->uniform_view, 1, GL_FALSE, attrib->data_view);
//    glUniformMatrix4fv(attrib->uniform_projection, 1, GL_FALSE, attrib->data_projection);
}

void render(void *data) {
    context *variable = (context *) data;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // bind textures on corresponding texture units

    texture_active_Bind(variable->data_texture);
    texture_active_Bind(variable->data_texture1);

    //draw
    glBindVertexArray(variable->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, attrib->data_count);
}

void finish(void *data) {
    context *variable = (context *) data;
    free(variable);
}

int main(int argc, char *argv[]) {

    return window(before_init, init, before_render, render, finish);
}