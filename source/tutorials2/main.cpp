
#include <SDL.h>
#include <stdio.h>
#include "angle_gl.h"
#include "Shader.h"

const unsigned int DISP_WIDTH = 800;
const unsigned int DISP_HEIGHT = 600;

/**
 * create the Vertex Buffer object(VBO) containing the given vertices.
 * @param vertices
 * @param numVertices
 * @return
 */
GLuint vboCreate(const void *vertices, GLuint numVertices){
    //create the vertex buffer object
    GLuint vbo;
    int nBuffers = 1;

    glGenBuffers(nBuffers,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //copy the vertex data in, and deactivate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * numVertices, vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        //failed
        glDeleteBuffers(nBuffers,&vbo);
        SDL_Log("Creating VBO failed, code %u\n", err);
        vbo = 0;
    }
    return vbo;
}
/**
 * Free the VBO
 * @param vbo
 */
void vboFree(GLuint vbo){
    glDeleteBuffers(1,&vbo);
}

/**
 * Encapsulates the data for a single vertex.
 * Must match the vertex shader's input
 */
typedef struct Vertex_s {
    float position[2];
} Vertex;


int main(int argc, char *argv[]) {

    // 1. Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //Setup the exit hook
    atexit(SDL_Quit);

    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 2. Create the window
    SDL_Window *window =  SDL_CreateWindow("SDL2",
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

    //Clear to black
    glClearColor(1.0f, 1.0f, 0.5f , 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Update the window
    SDL_GL_SwapWindow(window);

    //Activating the Shader
    GLuint shaderProg = shaderProgramsLoad("Simple2D.vert", "Simple2D.frag");
    if(!shaderProg){
        return EXIT_FAILURE;
    }
    glUseProgram(shaderProg);

    //Create the triangle
    const Vertex vertices[] = {
            {0.0f,-0.9f},
            {0.9f,0.9f},
            {-0.9f,0.9f},
            {0.5f,-0.5f},
            {0.5f,0.5f},
            {-0.5f,0.5f},
    };
    GLsizei vertSize = sizeof(vertices[0]);
    GLsizei numVertices = sizeof(vertices)/vertSize;
    GLuint triangleVBO = vboCreate(vertices,numVertices);
    if(!triangleVBO){
        //failed. Error message has already been printed, so just quit.
        return EXIT_FAILURE;
    }

    //set up for rendering the triangle (activate the vbo)
    GLuint positionIdx = 0; //Position is vertex attribute 0
    glBindBuffer(GL_ARRAY_BUFFER,triangleVBO);
    //GL_APICALL void GL_APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    glVertexAttribPointer(positionIdx,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)0);
    glEnableVertexAttribArray(positionIdx);

    // now draw
    //glDrawArrays(GL_LINES,0,numVertices);
    //glDrawArrays(GL_LINE_LOOP,0,numVertices);
    glDrawArrays(GL_TRIANGLES,0,numVertices);

    //Update the window
    SDL_GL_SwapWindow(window);

    // Wait for the user to quit
    bool quit = false;
    while(!quit){
        SDL_Event event;
        if(SDL_WaitEvent(&event)!=0){
            if(event.type == SDL_QUIT){
                quit = true;
            }
        }
    }

    //cleanup
    vboFree(triangleVBO);
    triangleVBO = 0;
    shaderProgramsDestroy(shaderProg);
    shaderProg = 0;

    return EXIT_SUCCESS;

}