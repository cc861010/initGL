#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "angle_gl.h"


size_t fileGetLength(FILE *file) {
    size_t length;
    size_t currPos = ftell(file);
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, currPos, SEEK_SET);
    return length;
}

static GLuint shaderLoad(const char *filename, GLenum shaderType) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        SDL_Log("Can't open file: %s\n", filename);
        return 0;
    }
    size_t length = fileGetLength(file);
    GLchar *shaderSrc = (GLchar *) calloc(length + 1, 1);
    if (!shaderSrc) {
        SDL_Log("out of memory when reading file: %s\n", filename);
        fclose(file);
        file = NULL;
    }

    fread(shaderSrc, 1, length, file);

    //Done with the file
    fclose(file);
    file = NULL;

    //create the shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar **) &shaderSrc, NULL);
    free(shaderSrc);
    shaderSrc = NULL;

    //compile the shader
    glCompileShader(shader);
    GLint compileSucceeded = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);
    if (!compileSucceeded) {
        //Compilation failed. Print error info
        SDL_Log("Compilation of shader %s failed:\n", filename);
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar * errLog = (GLchar *) malloc(logLength);
        if (errLog) {
            glGetShaderInfoLog(shader, logLength, &logLength, errLog);
            SDL_Log("%s\n", errLog);
            free(errLog);
        } else {
            SDL_Log("Couldn't get shader log; out of memory\n");
        }
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

/**
 * Destroys a shader
 * @param shaderID
 */
static void shaderDestroy(GLuint shaderID) {
    glDeleteShader(shaderID);
}


GLuint shader_programs_load(const char *vertFilename, const char *fragFilename) {
    GLuint vertShader = shaderLoad(vertFilename, GL_VERTEX_SHADER);
    if (!vertShader) {
        SDL_Log("couldn't load vertex shader: %s\n", vertFilename);
        return 0;
    }
    GLuint fragShader = shaderLoad(fragFilename, GL_FRAGMENT_SHADER);
    if (!fragShader) {
        SDL_Log("Couldn't load fragment shader: %s\n", fragFilename);
        shaderDestroy(vertShader);
        vertShader = 0;
        return 0;
    }

    GLuint shaderProg = glCreateProgram();
    if (shaderProg) {
        glAttachShader(shaderProg, vertShader);
        glAttachShader(shaderProg, fragShader);
        glLinkProgram(shaderProg);
        GLint linkingSucceeded = GL_FALSE;
        glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkingSucceeded);
        if (!linkingSucceeded) {
            SDL_Log("linking shader failed (vert shader: %s, frag shader: %s\n)", vertFilename, fragFilename);
            GLint logLength = 0;
            glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
            GLchar *errLog = (GLchar *) malloc(logLength);
            if (errLog) {
                glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);
                SDL_Log("%s\n", errLog);
                free(errLog);
            }else{
                SDL_Log("couldn't get shader link log: out of memory\n");
            }
            glDeleteProgram(shaderProg);
            shaderProg = 0;
        }
    }else{
        SDL_Log("Couldn't create shader program\n");
    }

    //Don't need these any more
    shaderDestroy(vertShader);
    shaderDestroy(fragShader);

    return shaderProg;

}


void shader_programs_free(GLuint shaderProgram){
    glDeleteProgram(shaderProgram);
}