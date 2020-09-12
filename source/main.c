
#include <SDL.h>
#include <stddef.h>
#include <stdbool.h>
#include "stb_image.h"
#include "angle_gl.h"
#include "Shader.h"
#include "Texture.h"


const unsigned int DISP_WIDTH = 800;
const unsigned int DISP_HEIGHT = 600;

/**
 * create the Vertex Buffer object(VBO) containing the given vertices.
 * @param vertices
 * @param numVertices
 * @return
 */
GLuint vboCreate(const void *vertices, GLuint numVertices) {
    //create the vertex buffer object
    GLuint vbo;
    int nBuffers = 1;

    glGenBuffers(nBuffers, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //copy the vertex data in, and deactivate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * numVertices, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        //failed
        glDeleteBuffers(nBuffers, &vbo);
        SDL_Log("Creating VBO failed, code %u\n", err);
        vbo = 0;
    }
    return vbo;
}

/**
 * Free the VBO
 * @param vbo
 */
void vboFree(GLuint vbo) {
    glDeleteBuffers(1, &vbo);
}


int main(int argc, char *argv[]) {

    // 1. Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //Setup the exit hook
    //atexit(SDL_Quit);

    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 2. Create the window
    SDL_Window *window = SDL_CreateWindow("SDL2",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't creat the main window", NULL);
        return EXIT_FAILURE;
    }

    // 3. Create the OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an opengl context", NULL);
        return EXIT_FAILURE;
    }


    //build and compile the Shader programs
    //-------------------------------------------------------------------
    GLuint shaderProg = shader_programs_load("Texture.vert", "Texture.frag");
    if (!shaderProg) {
        return EXIT_FAILURE;
    }

    //set up vertex data (and buffers) and configure vertex attributes
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    float vertices[] = {
            // positions            // colors               // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
            0, 1, 3,  //first triangle
            1, 2, 3   //second triangle
    };

    // load and generate the texture
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    GLuint textured1 = texture_load("google.png");
    if (!textured1) {
        return EXIT_FAILURE;
    }
    // ---VAO VBO EBO----------------------------------------------
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done onec)
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    glUseProgram(shaderProg);
    glUniform1i(glGetUniformLocation(shaderProg, "texture1"), 0);

    // Wait for the user to quit
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    // ------------------------------------------------------------
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        if (SDL_WaitEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // render
        // ------------------------------------------------------------
        // ------------------------------------------------------------
        // ------------------------------------------------------------
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        //bind texture on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textured1);

        glUseProgram(shaderProg);

        //render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        //Update the window
        SDL_GL_SwapWindow(window);

    }

    //cleanup
    vboFree(VBO);
    shader_programs_free(shaderProg);
    shaderProg = 0;
    texture_free(textured1);

    return EXIT_SUCCESS;

}