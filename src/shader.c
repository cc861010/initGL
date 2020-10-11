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

void shader_use(GLuint program_id) {
    glUseProgram(program_id);
}

void shader_setBool(GLuint program_id, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(program_id, name), (int) value);
}

void shader_setInt(GLuint program_id, const char *name, int value) {
    glUniform1i(glGetUniformLocation(program_id, name), value);
}

void shader_setFloat(GLuint program_id, const char *name, float value) {
    glUniform1f(glGetUniformLocation(program_id, name), value);
}

void shader_setVec2v(GLuint program_id, const char *name, void *value) {
    glUniform2fv(glGetUniformLocation(program_id, name), 1, (float *)value);
}

void shader_setVec2(GLuint program_id, const char *name, float x, float y) {
    glUniform2f(glGetUniformLocation(program_id, name), x, y);
}


void shader_setVec3v(GLuint program_id, const char *name, void *value) {
    glUniform3fv(glGetUniformLocation(program_id, name), 1, (float *)value);
}

void shader_setVec3(GLuint program_id, const char *name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(program_id, name), x, y, z);
}

void shader_setVec4v(GLuint program_id, const char *name, void *value) {
    glUniform4fv(glGetUniformLocation(program_id, name), 1, (float *)value);
}

void shader_setVec4(GLuint program_id, const char *name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(program_id, name), x, y, z, w);
}

void shader_setMat2(GLuint program_id, const char *name, void *value) {
    glUniformMatrix2fv(glGetUniformLocation(program_id, name), 1, GL_FALSE, (float *)value);
}

void shader_setMat3(GLuint program_id, const char *name, void *value) {
    glUniformMatrix3fv(glGetUniformLocation(program_id, name), 1, GL_FALSE, (float *)value);
}

void shader_setMat4(GLuint program_id, const char *name, void *value) {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name), 1, GL_FALSE, (float *)value);
}