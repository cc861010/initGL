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

void shader_use(GLuint program_id);

void shader_setBool(GLuint program_id, const char *name, bool value);

void shader_setInt(GLuint program_id, const char *name, int value);

void shader_setFloat(GLuint program_id, const char *name, float value);

void shader_setVec2v(GLuint program_id, const char *name, void *value);

void shader_setVec2(GLuint program_id, const char *name, float x, float y);

void shader_setVec3v(GLuint program_id, const char *name, void *value);

void shader_setVec3(GLuint program_id, const char *name, float x, float y, float z);

void shader_setVec4v(GLuint program_id, const char *name, void *value);

void shader_setVec4(GLuint program_id, const char *name, float x, float y, float z, float w);

void shader_setMat2(GLuint program_id, const char *name, void *value);

void shader_setMat3(GLuint program_id, const char *name, void *value);

void shader_setMat4(GLuint program_id, const char *name, void *value);


#endif