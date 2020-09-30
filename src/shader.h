#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdbool.h>
#include "angle_gl.h"


/**
 * load a vertex and fragment shader from disk and compiles (&links) them into a shader program
 * this will print any errors to the console.
 * @param vertFilename
 * @param fragFilename
 * @return
 */
GLuint shader_programs_load(const char *vertFilename, const char *fragFilename);

/**
 * Destroy a shader program
 * @param shaderProg
 */
void shader_programs_free(GLuint program_id);

void use(GLuint program_id) {
    glUseProgram(program_id);
}

void set_bool(GLuint program_id, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(program_id, name), (int) value);
}

void set_int(GLuint program_id, const char *name, int value) {
    glUniform1i(glGetUniformLocation(program_id, name), value);
}

void set_float(GLuint program_id, const char *name, float value) {
    glUniform1f(glGetUniformLocation(program_id, name), value);
}


#endif